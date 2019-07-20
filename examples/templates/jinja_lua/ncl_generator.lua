#!/usr/local/bin/lua

--- @module ncl_generator
-- Generates ncl final document based on template and using padding document.
-- It uses Lupa library which is a Jinja implementation made in Lua. 
package.path = package.path .. ';dependences/?.lua'
local lupa = require ("lupa")
local json = require ("json")

-- only necessary for a more beautiful output in debug mode
local pprint = require ("pprint")

--- deserialize_json Deserialize padding document.
-- It checks whether padding document is in correct extension and raise an error otherwise.
-- @param name padding document to deserialized
-- @return deserialize data
local function deserialize_json(name)

    local extension = name:match("^.+(%..+)$")
    local fileHandler = ''
    local content = ''
    local desData = ''
    if DEBUG then pprint(string.format("Padding extension: %s", extension)) end
    if extension ~= ".json" then
        error("Padding document in wrong format!!!") 
    else
        fileHandler = assert(io.open(name, "r"))
        content = fileHandler:read("*all")
        desData = json:decode(content) 
        fileHandler:close()
    end
    return desData
end

--- mysplit split name using sep as separator.
-- @param imputstr string.
-- @param sep delimeter string.
-- @return table containing substrings on inputstr.
function mysplit(inputstr, sep)
    if sep == nil then
            sep = "%s"
    end
    local t={}
    for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
            table.insert(t, str)
    end
    return t
end

--- handle_name Generate final ncl-document name.
--  @param template_doc name of template families.
--  @return name based on template_doc
local function handle_name(template_doc)
    name_table = mysplit(template_doc, '.')
    if string.gmatch(name_table[1], 'child') then
        name_table = mysplit(name_table[1], '_')
    end
    if string.gmatch(name_table[1], 'base') then
        name_table = mysplit(name_table[1], '_') 
    end
    return name_table[1]
end

--- main handles templating processing using Lupa pure Lua Jinja implementation.
-- Enable debud mode whether "-d" is passed as arguments.
-- @param 
-- @return 
local function main ()

    if #arg == 3 and arg[3] == '-d' then
        DEBUG = true
        pprint("DEBUG mode on...")
    else
        DEBUG = false
    end

    local padding_doc = arg[1]
    local template_doc = arg[2]

    if DEBUG then
        log_initial = string.format("Starting generating ncl document from %s with %s", template_doc, padding_doc)
        pprint(log_initial)
    end

    local env = lupa.configure{
                loader=lupa.loaders.filesystem('./templates'),
                trim_blocks=true,
                lstrip_blocks=true
    }

    local context = deserialize_json(padding_doc)

    if DEBUG then
        pprint(context)
    end

    content = {files_list = context}
    ncl = lupa.expand_file(template_doc, content)

    ncl_filename = handle_name(template_doc)
    if DEBUG then
        pprint(string.format("filename: %s", ncl_filename))
    end
    ncl_doc = io.open(ncl_filename .. '.ncl', "w")
    ncl_doc:write(ncl)
end

main ()