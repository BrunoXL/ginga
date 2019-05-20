from jinja2 import Environment, FileSystemLoader, select_autoescape
from pathlib import Path
import os 

def get_media(path):
    """ Gets all files from path and save them on a list
    
    :param path: absolute path for this script
    return: 
    """
    files = os.listdir(path=path)
    if len(files) == 0:
        raise Exception(path) 
    files = [path.joinpath(i) for i in files]
    return files


if __name__ == "__main__":
    abs_path = Path.cwd()
    env = Environment(
        loader=FileSystemLoader(str(abs_path.joinpath('templates'))),
        autoescape=select_autoescape(['ncl', 'xml']),
        trim_blocks=True,
        lstrip_blocks=True
    )

    template = env.get_template('slide_show.j2')

    path = abs_path.joinpath('media')

    try:
        context = {'list_files': get_media(path)}
    except Exception as e:
        print ("Error: No media associated to", e.args[0])
    else:
        content = template.render(context)
        path = abs_path.joinpath('slide_show.ncl')   
        with open(path , "w") as file:
            file.write(content)
