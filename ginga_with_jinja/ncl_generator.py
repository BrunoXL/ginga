from jinja2 import Environment, FileSystemLoader, select_autoescape, meta
from pathlib import Path
import json
import re

def deserialize_json(name):
    """ Deserealize a json file given as padding document 
        Information retrieves from it is used to fill in
        jinja templates
    
    :param name: name of padding document
    """
    if Path(name).suffix != '.json':
        raise Exception(name)
    with open(name, "r") as file:    
        des_json = json.load(file)
    return des_json
      

if __name__ == "__main__":
    abs_path = Path.cwd()
    #TODO: get name from value passed on command line
    padding_doc = "padding.json"

    env = Environment(
        loader=FileSystemLoader(str(abs_path.joinpath('templates'))),
        autoescape=select_autoescape(['ncl', 'xml']),
        trim_blocks=True,
        lstrip_blocks=True
    )

    #TODO:find a way to pass the 'younger' child template
    
    template = env.get_template('medias.ncl.j2')

    try:
        context = {'files_list': deserialize_json(padding_doc)}
        print (context)
    except Exception as e:
        msg  = "Error: Padding document '{e}' in wrong format".format(e=e.args[0])
        print (msg)
    else:
        content = template.render(context)
        path = abs_path.joinpath('slideShow.ncl')   
        with open(path , "w") as file:
            file.write(content)
