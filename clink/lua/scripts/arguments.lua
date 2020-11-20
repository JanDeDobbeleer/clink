-- Copyright (c) 2016 Martin Ridgers
-- License: http://opensource.org/licenses/MIT

--------------------------------------------------------------------------------
local _arglink = {}
_arglink.__index = _arglink
setmetatable(_arglink, { __call = function (x, ...) return x._new(...) end })

--------------------------------------------------------------------------------
function _arglink._new(key, matcher)
    return setmetatable({
        _key = key,
        _matcher = matcher,
    }, _arglink)
end



--------------------------------------------------------------------------------
local _argreader = {}
_argreader.__index = _argreader
setmetatable(_argreader, { __call = function (x, ...) return x._new(...) end })

--------------------------------------------------------------------------------
function _argreader._new(root)
    local reader = setmetatable({
        _matcher = root,
        _arg_index = 1,
        _stack = {},
        _word_types = nil,
    }, _argreader)
    return reader
end

--------------------------------------------------------------------------------
function _argreader:update(word)
    local arg_match_type = "a" --arg

    -- Check for flags and switch matcher if the word is a flag.
    local matcher = self._matcher
    local is_flag = matcher:_is_flag(word)
    if is_flag then
        if matcher._flags then
            self:_push(matcher._flags)
            arg_match_type = "f" --flag
        else
            return
        end
    end

    matcher = self._matcher
    local arg_index = self._arg_index
    local arg = matcher._args[arg_index]

    arg_index = arg_index + 1

    -- If arg_index is out of bounds we should loop if set or return to the
    -- previous matcher if possible.
    if arg_index > #matcher._args then
        if matcher._loop then
            self._arg_index = math.min(math.max(matcher._loop, 1), #matcher._args)
        elseif not self:_pop() then
            self._arg_index = arg_index
        end
    else
        self._arg_index = arg_index
    end

    -- Some matchers have no args at all.
    if not arg then
        self:_add_word_type("n") --none
        return
    end

    -- Parse the word type.
    if self._word_types then
        local t = "o" --other
        if arg._links and arg._links[word] then
            t = arg_match_type
        else
            for _, i in ipairs(arg) do
                if type(i) == "string" and i == word then
                    t = arg_match_type
                    break
                end
            end
        end
        self:_add_word_type(t)
    end

    -- Does the word lead to another matcher?
    for key, linked in pairs(arg._links) do
        if key == word then
            self:_push(linked)
            break
        end
    end
end

--------------------------------------------------------------------------------
function _argreader:_push(matcher)
    table.insert(self._stack, { self._matcher, self._arg_index })
    self._matcher = matcher
    self._arg_index = 1
end

--------------------------------------------------------------------------------
function _argreader:_pop()
    if #self._stack > 0 then
        self._matcher, self._arg_index = table.unpack(table.remove(self._stack))
        return true
    end

    return false
end

--------------------------------------------------------------------------------
function _argreader:_add_word_type(t)
    if self._word_types then
        table.insert(self._word_types, t)
    end
end



--------------------------------------------------------------------------------
local _argmatcher = {}
_argmatcher.__index = _argmatcher
setmetatable(_argmatcher, { __call = function (x, ...) return x._new(...) end })

--------------------------------------------------------------------------------
function _argmatcher._new()
    local matcher = setmetatable({
        _args = {},
    }, _argmatcher)
    matcher._flagprefix = {}
    return matcher
end

--------------------------------------------------------------------------------
--- -name:  _argmatcher:addarg
--- -arg:   choices...:string|table
--- -ret:   self
--- -show:  local my_parser = clink.argmatcher("git")
--- -show:  :addarg("add", "status", "commit", "checkout")
--- This adds argument matches.  Arguments can be a string, a string linked to
--- another parser by the concatenation operator, a table of arguments, or a
--- function that returns a table of arguments.  See <a
--- href="#argumentcompletion">Argument Completion</a> for more information.
function _argmatcher:addarg(...)
    local list = { _links = {} }
    self:_add(list, {...})
    table.insert(self._args, list)
    return self
end

--------------------------------------------------------------------------------
--- -name:  _argmatcher:addflags
--- -arg:   flags...:string
--- -ret:   self
--- -show:  local my_parser = clink.argmatcher("git")
--- -show:  :addarg({ "add", "status", "commit", "checkout" })
--- -show:  :addflags("-a", "-g", "-p", "--help")
--- This adds flag matches.  Flags are separate from arguments:  When listing
--- possible completions for an empty word, only arguments are listed.  But when
--- the word being completed starts with the first character of any of the
--- flags, then only flags are listed.  See <a
--- href="#argumentcompletion">Argument Completion</a> for more information.
function _argmatcher:addflags(...)
    local flag_matcher = self._flags or _argmatcher()
    local list = flag_matcher._args[1] or { _links = {} }
    local prefixes = self._flagprefix or {}

    flag_matcher:_add(list, {...}, prefixes)

    flag_matcher._args[1] = list
    self._flags = flag_matcher

    if not self._deprecated then
        self._flagprefix = prefixes
    end
    return self
end

--------------------------------------------------------------------------------
--- -name:  _argmatcher:loop
--- -arg:   [index:integer]
--- -ret:   self
--- -show:  clink.argmatcher("xyzzy")
--- -show:  :addarg("zero", "cero")     -- first arg can be zero or cero
--- -show:  :addarg("one", "uno")       -- second arg can be one or uno
--- -show:  :addarg("two", "dos")       -- third arg can be two or dos
--- -show:  :loop(2)    -- fourth arg loops back to position 2, for one or uno, and so on
--- This makes the parser loop back to argument position <em>index</em> when it
--- runs out of positional sets of arguments (if <em>index</em> is omitted it
--- loops back to argument position 1).
function _argmatcher:loop(index)
    self._loop = index or -1
    return self
end

--------------------------------------------------------------------------------
--- -name:  _argmatcher:setflagprefix
--- -arg:   [prefixes...:string]
--- -ret:   self
--- -show:  local my_parser = clink.argmatcher()
--- -show:  :setflagprefix("-", "/", "+")
--- -show:  :addflags("--help", "/?", "+mode")
--- -deprecated: _argmatcher:addflags
--- This overrides the default flag prefix (<code>-</code>).  The flag prefixes are used to
--- switch between matching arguments versus matching flags.  When listing
--- possible completions for an empty word (e.g. <code>command _</code> where the cursor is
--- at the <code>_</code>), only arguments are listed.  And only flags are listed when the
--- word starts with one of the flag prefixes.  Each flag prefix must be a
--- single character, but there can be multiple prefixes.<br/>
--- <br/>
--- This is no longer needed because <code>:addflags()</code> does it
--- automatically.
function _argmatcher:setflagprefix(...)
    if self._deprecated then
        local old = self._flagprefix
        self._flagprefix = {}
        for _, i in ipairs({...}) do
            if type(i) ~= "string" or #i ~= 1 then
                error("Flag prefixes must be single character strings", 2)
            end
            self._flagprefix[i] = old[i] or 0
        end
    end
    return self
end

--------------------------------------------------------------------------------
--- -name:  _argmatcher:nofiles
--- -ret:   self
--- This makes the parser prevent invoking <a href="#matchgenerators">match
--- generators</a>.  You can use it to "dead end" a parser and suggest no
--- completions.
function _argmatcher:nofiles()
    self._no_file_generation = true
    return self
end

--------------------------------------------------------------------------------
function _argmatcher.__concat(lhs, rhs)
    if getmetatable(rhs) ~= _argmatcher then
        error("Right-hand side must be an argmatcher object", 2)
    end

    local t = type(lhs)
    if t == "string" then
        return _arglink(lhs, rhs)
    end

    if t == "table" then
        local ret = {}
        for _, i in ipairs(lhs) do
            table.insert(ret, i .. rhs)
        end
        return ret
    end

    error("Left-hand side must be a string or a table of strings", 2)
end

--------------------------------------------------------------------------------
function _argmatcher:__call(arg)
    if type(arg) ~= "table" then
        error("Shorthand matcher arguments must be tables", 2)
    end

    local is_flag
    is_flag = function(x)
        local is_link = (getmetatable(x) == _arglink)
        if type(x) == "table" and not is_link then
            return is_flag(x[1])
        end

        if is_link then
            x = x._key
        end

        if self:_is_flag(tostring(x)) then
            return true
        end

        if x then
            local first_char = x:sub(1, 1)
            if first_char and first_char:match("[-/]") then
                return true
            end
        end

        return false
    end

    if is_flag(arg[1]) then
        return self:addflags(table.unpack(arg))
    end

    return self:addarg(table.unpack(arg))
end

--------------------------------------------------------------------------------
function _argmatcher:_is_flag(word)
    local first_char = word:sub(1, 1)
    for i, _ in pairs(self._flagprefix) do
        if first_char == i then
            return true
        end
    end

    return false
end

--------------------------------------------------------------------------------
local function add_prefix(prefixes, string)
    if string and type(string) == "string" then
        local prefix = string:sub(1, 1)
        if prefix:len() > 0 then
            if prefix:match('[A-Za-z]') then
                error("flag string '"..string.."' is invalid because it starts with a letter and would interfere with argument matching.")
            else
                prefixes[prefix] = (prefixes[prefix] or 0) + 1
            end
        end
    end
end

--------------------------------------------------------------------------------
function _argmatcher:_add(list, addee, prefixes)
    -- Flatten out tables unless the table is a link
    local is_link = (getmetatable(addee) == _arglink)
    if type(addee) == "table" and not is_link and not addee.match then
        if getmetatable(addee) == _argmatcher then
            for _, i in ipairs(addee._args) do
                for _, j in ipairs(i) do
                    table.insert(list, j)
                    if prefixes then add_prefix(prefixes, j) end
                end
                if i._links then
                    for k, m in pairs(i._links) do
                        list._links[k] = m
                        if prefixes then add_prefix(prefixes, k) end
                    end
                end
            end
        else
            for _, i in ipairs(addee) do
                self:_add(list, i, prefixes)
            end
        end
        return
    end

    if is_link then
        list._links[addee._key] = addee._matcher
        if prefixes then add_prefix(prefixes, addee._key) end
    else
        table.insert(list, addee)
        if prefixes then add_prefix(prefixes, addee) end
    end
end

--------------------------------------------------------------------------------
function _argmatcher:_generate(line_state, match_builder)
    local reader = _argreader(self)

    -- Consume words and use them to move through matchers' arguments.
    local word_count = line_state:getwordcount()
    for word_index = 2, (word_count - 1) do
        local word = line_state:getword(word_index)
        reader:update(word)
    end

    -- There should always be a matcher left on the stack, but the arg_index
    -- could be well out of range.
    local matcher = reader._matcher
    local arg_index = reader._arg_index

    -- Are we left with a valid argument that can provide matches?
    local add_matches = function(arg)
        for key, _ in pairs(arg._links) do
            match_builder:addmatch(key, "arg")
        end

        for _, i in ipairs(arg) do
            if type(i) == "function" then
                local j = i(line_state:getendword(), word_count, line_state, match_builder)
                if type(j) ~= "table" then
                    return j or false
                end

                match_builder:addmatches(j, "arg")
            else
                match_builder:addmatch(i, "arg")
            end
        end

        return true
    end

    -- Select between adding flags or matches themselves. Works in conjunction
    -- with getwordbreakinfo()'s return.
    if matcher._flags and matcher:_is_flag(line_state:getendword()) then
        add_matches(matcher._flags._args[1])
        return true
    else
        local arg = matcher._args[arg_index]
        if arg then
            return add_matches(arg) and true or false
        end
    end

    -- No valid argument. Decide if we should match files or not.
    local no_files = matcher._no_file_generation or #matcher._args == 0
    return no_files
end

--------------------------------------------------------------------------------
-- Deprecated.
function _argmatcher:add_arguments(...)
    self:addarg(...)
    return self
end

--------------------------------------------------------------------------------
-- Deprecated.
function _argmatcher:add_flags(...)
    self:addflags(...)
    return self
end

--------------------------------------------------------------------------------
-- Deprecated.  This was an undocumented function, but some scripts found it and
-- used it anyway.  The compatibility shim tries to make them work essentially
-- the same as in 0.4.8, but it may not be exactly accurate.
function _argmatcher:flatten_argument(index)
    local t = {}

    if index > 0 and index <= #self._args then
        local args = self._args[index]
        for _, i in ipairs(args) do
            if type(i) == "string" then
                table.insert(t, i)
            end
        end
        if args._links then
            for k, _ in pairs(args._links) do
                table.insert(t, k)
            end
        end
    end

    return t
end

--------------------------------------------------------------------------------
-- Deprecated.
function _argmatcher:set_arguments(...)
    self._args = { _links = {} }
    self:addarg(...)
    return self
end

--------------------------------------------------------------------------------
-- Deprecated.
function _argmatcher:set_flags(...)
    self._flags = nil
    self:addflags(...)
    return self
end



--------------------------------------------------------------------------------
clink = clink or {}
local _argmatchers = {}

--------------------------------------------------------------------------------
--- -name:  clink.argmatcher
--- -arg:   [priority:integer]
--- -arg:   commands...:string
--- -ret:   <a href="#_argmatcher">_argmatcher</a>
--- Creates and returns a new argument matcher parser object.  Use <a
--- href="#_argmatcher:addarg">:addarg()</a> and etc to add arguments, flags,
--- other parsers, and more.  See <a href="#argumentcompletion">Argument
--- Completion</a> for more information.
function clink.argmatcher(...)
    local matcher = _argmatcher()

    -- Extract priority from the arguments.
    matcher._priority = 999
    local input = {...}
    if #input > 0 and type(input[1]) == "number" then
        matcher._priority = input[1]
        table.remove(input, 1)
    end

    -- Register the argmatcher
    for _, i in ipairs(input) do
        _argmatchers[i:lower()] = matcher
    end

    return matcher
end



--------------------------------------------------------------------------------
local function _find_argmatcher(line_state)
    -- Running an argmatcher only makes sense if there's two or more words.
    if line_state:getwordcount() < 2 then
        return
    end

    local first_word = line_state:getword(1)

    -- Check for an exact match.
    local argmatcher = _argmatchers[path.getname(first_word):lower()]
    if argmatcher then
        return argmatcher
    end

    -- If the extension is in PATHEXT then try stripping the extension.
    local ext = path.getextension(first_word):lower()
    if ext and ext ~= "" then
        if (";"..clink.get_env("pathext")..";"):lower():match(";"..ext..";", 1, true) then
            argmatcher = _argmatchers[path.getbasename(first_word):lower()]
            if argmatcher then
                return argmatcher
            end
        end
    end
end



------------------------------------------------------------------------------
function clink._parse_word_types(line_state)
    local parsed_word_types = {}

    local word_count = line_state:getwordcount()
    local first_word = line_state:getword(1) or ""
    if word_count > 1 or string.len(first_word) > 0 then
        if string.len(os.getalias(first_word) or "") > 0 then
            table.insert(parsed_word_types, "d"); --doskey
        else
            table.insert(parsed_word_types, "c"); --command
        end
    end

    local argmatcher = _find_argmatcher(line_state)
    if argmatcher then
        local reader = _argreader(argmatcher)
        reader._word_types = parsed_word_types

        -- Consume words and use them to move through matchers' arguments.
        for word_index = 2, word_count do
            local word = line_state:getword(word_index)
            reader:update(word)
        end
    end

    local s = ""
    for _, t in ipairs(parsed_word_types) do
        s = s..t
    end

    return s
end



--------------------------------------------------------------------------------
local argmatcher_generator = clink.generator(24)

--------------------------------------------------------------------------------
function argmatcher_generator:generate(line_state, match_builder)
    local argmatcher = _find_argmatcher(line_state)
    if argmatcher then
        return argmatcher:_generate(line_state, match_builder)
    end

    return false
end

--------------------------------------------------------------------------------
function argmatcher_generator:getwordbreakinfo(line_state)
    local argmatcher = _find_argmatcher(line_state)
    if argmatcher then
        local reader = _argreader(argmatcher)

        -- Consume words and use them to move through matchers' arguments.
        local word_count = line_state:getwordcount()
        for word_index = 2, (word_count - 1) do
            local word = line_state:getword(word_index)
            reader:update(word)
        end

        -- There should always be a matcher left on the stack, but the arg_index
        -- could be well out of range.
        argmatcher = reader._matcher
        if argmatcher and argmatcher._flags then
            local word = line_state:getendword()
            if argmatcher:_is_flag(word) then
                return 0, 1
            end
        end
    end

    return 0
end



--------------------------------------------------------------------------------
clink.arg = clink.arg or {}

--------------------------------------------------------------------------------
local function starts_with_flag_character(parser, part)
    if part == nil then
        return false
    end

    local prefix = part:sub(1, 1)
    return parser._flagprefix[prefix] and true or false
end

--------------------------------------------------------------------------------
local function parser_initialise(parser, ...)
    for _, word in ipairs({...}) do
        local t = type(word)
        if t == "string" then
            parser:addflags(word)
        elseif t == "table" then
            if getmetatable(word) == _arglink and starts_with_flag_character(parser, word._key) then
                parser:addflags(word)
            else
                parser:addarg(word)
            end
        else
            error("Additional arguments to new_parser() must be tables or strings", 2)
        end
    end
end

--------------------------------------------------------------------------------
--- -name:  clink.arg.new_parser
--- -arg:   ...
--- -ret:   table
--- -show:  -- Deprecated form:
--- -show:  local parser = clink.arg.new_parser(
--- -show:  &nbsp; { "abc", "def" },       -- arg position 1
--- -show:  &nbsp; { "ghi", "jkl" },       -- arg position 2
--- -show:  &nbsp; "--flag1", "--flag2"    -- flags
--- -show:  )<br/>
--- -show:  -- Replace with form:
--- -show:  local parser = clink.argmatcher()
--- -show:  :addarg("abc", "def")               -- arg position 1
--- -show:  :addarg("ghi", "jkl")               -- arg position 2
--- -show:  :addflags("--flag1", "--flag2")     -- flags
--- -deprecated: clink.argmatcher
--- Creates a new parser and adds <em>...</em> to it.
function clink.arg.new_parser(...)
    local parser = clink.argmatcher()
    parser._deprecated = true
    parser._flagprefix = {}
    parser._flagprefix['-'] = 0
    if ... then
        local success, msg = xpcall(parser_initialise, _error_handler_ret, parser, ...)
        if not success then
            error(msg, 2)
        end
    end
    return parser
end

--------------------------------------------------------------------------------
--- -name:  clink.arg.register_parser
--- -arg:   cmd:string
--- -arg:   parser:table
--- -ret:   table
--- -show:  -- Deprecated form:
--- -show:  local parser1 = clink.arg.new_parser("abc", "def")
--- -show:  local parser2 = clink.arg.new_parser("ghi", "jkl")
--- -show:  clink.arg.register_parser("foo", parser1)
--- -show:  clink.arg.register_parser("foo", parser2)<br/>
--- -show:  -- Replace with new form:
--- -show:  clink.argmatcher("foo"):addarg(parser1, parser2)<br/>
--- -show:  -- Warning:  Note that the following are NOT the same as above!
--- -show:  -- This replaces parser1 with parser2:
--- -show:  clink.argmatcher("foo"):addarg(parser1)
--- -show:  clink.argmatcher("foo"):addarg(parser2)
--- -show:  -- This uses only parser2 if/when parser1 finishes parsing args:
--- -show:  clink.argmatcher("foo"):addarg(parser1):addarg(parser2)
--- -deprecated: clink.argmatcher
--- Adds <em>parser</em> to the first argmatcher for <em>cmd</em>.  This behaves
--- similarly to v0.4.8, but not identically.  The Clink schema has changed
--- significantly enough that there is no direct 1:1 translation.  Calling
--- <code>clink.arg.register_parser</code> repeatedly with the same command to
--- merge parsers is not supported anymore.
function clink.arg.register_parser(cmd, parser)
    if _argmatchers[cmd:lower()] then
        error("clink.arg.register_parser() is deprecated and can no longer merge parsers by repeatedly calling register_parser for the same command.")
        return
    end

    if parser and getmetatable(parser) == _argmatcher then
        if not parser._deprecated then
            error("clink.arg.register_parser() is deprecated and can only be used with parsers created by clink.arg.new_parser().")
            return
        end

        _argmatchers[cmd:lower()] = parser
        return parser
    end

    local matcher = clink.arg.new_parser(parser)
    _argmatchers[cmd:lower()] = matcher
    return matcher
end
