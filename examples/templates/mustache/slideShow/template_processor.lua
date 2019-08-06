#!/usr/local/bin/lua

--- @module ncl_generator
-- Generates ncl final document based on template and using padding document.
-- It uses Lustache a pure Lua Mustache implementation.
-- Lustache main module should be kept in dependences folder 
-- whilst it dependences must be in lustache subfoder  

-- only necessary for a more beautiful output in debug mode
package.path = package.path .. ';dependences/?.lua'
local lustache = require ("lustache") 
local json = require ("json")
local pprint = require ("pprint")


--- deserialize_json Deserialize padding document.templates..
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
    for str in string.gmatch(inputstr, "(.-)("..sep..")") do
            table.insert(t, str)
    end
    return t
end

--- handle_template handle mustache templates using sep as separator.
-- @param template_doc string.
-- @return string with information ftom the opened template.
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

--- main handles templating processing using Lupa pure Lua Jinja implementation.
-- Enable debud mode whether "-d" is passed as arguments.
-- @param 
-- @return 
local function main (data, debug)

    DEBUG = debug or true

    local padding_doc = data.padding
    local template_doc = data.template
    local engine = data.engine

    if DEBUG then
        log_initial = string.format("Starting generating ncl document from %s with %s on %s template engine",
                                    template_doc, padding_doc, engine )
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

    -- Grab content from templates
    local i = 0
    local baseTemplateData = handle_template(template_doc)
    local templatesData = {}
    local partials = {}
    --name of the files must be same as the partials in the main template document,
    --otherwise logic will fail!!!!!!!!!!!!!
    for i = 1, #data.partial do
        templatesData[i] = handle_template(data.partial[i])
        local name = mysplit(data.partial[i], data.partial[i]:match(".mustache"))
        partials[name[1]] = templatesData[i]
    end
    if DEBUG then 
        pprint("Template Data")
        pprint(templatesData)
    end

    if DEBUG then
        pprint("Partials table:")
        pprint(partials)
    end

    output = lustache:render(baseTemplateData, context, partials)

    if DEBUG then
        pprint(output)
    end
    
    ncl_doc = mysplit(template_doc, template_doc:match(".mustache"))
    pprint(string.format("ncl_doc %s",ncl_doc[1]))
    fh = assert(io.open(ncl_doc[1], "w"))
    fh:write(output)
    fh:close()
end

local data = {}

function handler(evt)
    if  evt.class == 'ncl' and evt.type == 'attribution' and evt.action == 'start' then
            local i = 0 
        if evt.name == 'template' then
            data.template = evt.value
            pprint("----Template document-------")
            pprint(data.template)
        elseif evt.name == 'padding' then 
            data.padding = evt.value
            pprint("----Padding document-------")
            pprint(data.padding)
        elseif evt.name == 'mustache' then
            data.engine = evt.value
            pprint("----Mustache Template Engine selected-------")
            pprint(data.engine)
        elseif evt.name:match("partial") == 'partial' then
            table.insert(data.partial, evt.name)
            pprint("----Partial -------")
            pprint(table.partial[i])
        end
    end
end

event.register(handler)

data.template = "slideShow.ncl.mustache"
data.padding = "padding.json"
data.engine = "mustache"
data.partial = {}
table.insert (data.partial,"links.mustache")
table.insert (data.partial,"medias.mustache")

pprint(data)

main(data)

event.post('in', {
    class  = 'ncl',
    type   = 'presentation',
    action = 'stop'
})

pprint("Finished Template Processing")