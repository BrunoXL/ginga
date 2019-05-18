from jinja2 import Environment, FileSystemLoader, select_autoescape
import os

def get_media(path):
    files = os.listdir(path=path)
    if files == None:  
        return 
    files = [ path + '/' + i for i in files ]
    return files

if __name__ == "__main__":
    env = Environment(
        loader=FileSystemLoader('/home/bruno/repos/ginga_with_jinja/templates'),
        # autoescape=select_autoescape(['ncl', 'xml']),
        trim_blocks=True,
        lstrip_blocks=True
    )

    template = env.get_template('slide_show.j2')

    path = 'media'

    context = {'list_files': get_media(path)}
    
    print (template.render(context))

    content = template.render(context)

    with open('example.ncl', "w") as file:
        file.write( content)