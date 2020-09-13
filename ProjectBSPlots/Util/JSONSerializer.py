from os import path
import json

def SerializeJSON(data : object, filename : str) :
    mode = "w" if path.exists(filename) else "x"
    with open(filename,mode) as json_file:
        json.dump(data,json_file, default=lambda x: x.__dict__)

def DeserializeJSON(filename : str) -> object :
    with open(filename) as json_file:
        return json.load(json_file)
