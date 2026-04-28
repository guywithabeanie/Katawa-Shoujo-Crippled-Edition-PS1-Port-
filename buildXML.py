import xml.etree.ElementTree as ElementTree
import os
from pathlib import Path
import sys

def recursivelyParseFolder( directory: Path, parentNode: ElementTree.SubElement ):
    for x in os.listdir( directory ):
        path = directory / x

        #Remove the root directory from the path
        useablePath = Path( *path.parts[1:] )

        if path.is_file() and path.suffix != ".xml":
            file = ElementTree.SubElement(parentNode, "file", {
                "name" : x,
                "type" : "data",
                "source" : str( useablePath )
            })
        
        elif path.is_dir():
            dir = ElementTree.SubElement(parentNode, "dir", {
                "name" : x,
                "type" : "data",
                "source" : str( useablePath )
            })
            recursivelyParseFolder( path, dir if any( path.iterdir() ) else parentNode )

    return

config = {
    "directory" : "",
    "projectName" : "",
    "volume" : "PS1DISK",
    "publisher" : "MINGEBAG"
}

argv = sys.argv
i = 1

while i < len( argv ):
    arg = argv[i]

    if arg in ("-cd", "-proj", "-v", "-pub"):
        i += 1
        if i >= len(argv):
            raise ValueError(f"Missing value for {arg}")

        if arg == "-cd":
            config["directory"] = argv[i]
        elif arg == "-proj":
            config["projectName"] = argv[i]
        elif arg == "-v":
            config["volume"] = argv[i]
        elif arg == "-pub":
            config["publisher"] = argv[i]

    i += 1

if config["directory"] == "" or config["projectName"] == "":
    print("Usage : generateXML.py -cd CdDirectory -proj ProjectName [-v VolumeName] [-pub Publisher]")
    sys.exit(1)

root = ElementTree.Element("iso_project", {
    "image_name" : f"bin/{config['projectName']}.bin",
    "cue_sheet"  : f"bin/{config['projectName']}.cue"
})

tracks = ElementTree.SubElement(root, "track", {
    "type" : "data"
})

identifiers = ElementTree.SubElement(tracks, "identifiers", {
	"system"        : "PLAYSTATION",
	"application"   : "PLAYSTATION",
	"volume"        : config["volume"],
	"volume_set"    : config["volume"],
	"publisher"     : config["publisher"],
	"data_preparer" : "MKPSXISO"
})

directoryTree = ElementTree.SubElement(tracks, "directory_tree")

rootDirectory = Path( config["directory"] )
print( rootDirectory )

recursivelyParseFolder( rootDirectory, directoryTree )

tree = ElementTree.ElementTree(root)

ElementTree.indent(root, space="    ")

tree.write( f"{config['directory']}/isoconfig.xml", xml_declaration=True, encoding="UTF-8" )
