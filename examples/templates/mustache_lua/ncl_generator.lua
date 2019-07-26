#!/usr/local/bin/lua

--- @module ncl_generator
-- Generates ncl final document based on template and using padding document.
-- It uses Lustache (Lua mustache engine implementation).
package.path = package.path .. ';dependences/?.lua'
local lustache = require ("lustache")
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

local function handle_template(template_doc)
    local extension = template_doc:match("^.+(%..+)$")
    local fileHandler = nil
    if DEBUG then pprint(string.format("Template extension of %s: %s", template_doc, extension)) end
    if extension ~= ".mustache" then
        error("Template document in wrong format!!!") 
    else
        fileHandler = assert(io.open("templates/" .. template_doc, "r"))
        content = fileHandler:read("*all")
        fileHandler:close()
    end
    return content
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
    for str in string.gmatch(inputstr, "(.-)("..sep..")") do
            table.insert(t, str)
    end
    return t
end

--- main handles templating processing using implementation.
-- Enable debud mode whether "-d" is passed as arguments.
-- In mustache template must be strings with mustache tags.
-- @param 
-- @return 
local function main ()
    
    local padding_doc = arg[1]
    local baseTemplateDoc = arg[2]
    
    -- local padding_doc = "padding.json"
    -- local baseTemplateDoc = "slideShow.ncl.mustache"

    local param = {}
    local i = 1
    for key, value  in pairs(arg) do
        if key > 2 then
            param[i] = value    
            i = i + 1
        end
    end

    if param[#param] == '-d' then
        DEBUG = true
        pprint("DEBUG mode on...")
    else
        DEBUG = false
    end

    table.remove(param, #param)

    if DEBUG then
        pprint("templates listing:")
        pprint(param)
    end

    
    -- Grab content from templates
    local baseTemplateData = handle_template(baseTemplateDoc)
    local templatesData = {}
    for i = 1, #param do
        templatesData[i] = handle_template(param[i])
    end

    if DEBUG then 
        pprint(templateData)
    end

    if DEBUG then
        log_initial = string.format("Starting generating ncl document from %s with %s", baseTemplateDoc, padding_doc)
        pprint(log_initial)
    end

    local content = deserialize_json(padding_doc)

    local i = 0 
    local index = {}
    local next = {}
    for i=1, #content[1].contents do
        index[i] = i
        content[1].contents[i].index = index[i]
        if i ~= #content[1].contents then
            next[i] = i+1 
            content[1].contents[i].next = next[i]
        end
    end  

    context = content[1]

    if DEBUG then
        pprint(context)
    end
    
    local partials = {}
    --name of the files must be same as the partials in the main template document,
    --otherwise logic will fail!!!!!!!!!!!!!
    for i = 1, #templatesData do
        local name = mysplit(param[i], param[i]:match(".mustache"))
        partials[name[1]] = templatesData[i]
    end

    if DEBUG then
        pprint("Partials table:")
        pprint (partials)
    end

    output = lustache:render(baseTemplateData, context,partials)

    if DEBUG then
        pprint(output)
    end

    ncl_doc = mysplit(baseTemplateDoc, baseTemplateDoc:match(".mustache"))
    fh = io.open(ncl_doc[1], "w")
    fh.write(fh, output)
end

main()