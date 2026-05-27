from argparse import ArgumentParser
from pathlib import Path
import xml.etree.ElementTree as ET
import os


def parse_folders(root: Path, directory: Path, parentNode: ET.SubElement):
    """
    Recursively adds files/directories to the parent node.

    Args:
        root (pathlib.Path): The root directory for the script.
        directory (pathlib.Path): The directory to parse.
        node (xml.etree.ET.SubElement): The parenting node to append to.
    """
    for x in os.listdir(directory):
        path = directory / x

        # Remove the root directory from the path
        useablePath = path.resolve().relative_to(root.resolve())

        if path.is_file() and path.suffix != ".xml":
            file = ET.SubElement(
                parentNode,
                "file",
                {"name": x, "type": "data", "source": str(useablePath)},
            )

        elif path.is_dir():
            dir = ET.SubElement(
                parentNode,
                "dir",
                {"name": x, "type": "data", "source": str(useablePath)},
            )
            parse_folders(root, path, dir if any(path.iterdir()) else parentNode)


def parse_cmd():

    parser = ArgumentParser(
        prog="buildXML.py", description="Generates XML files for use with mkpsxiso."
    )

    parser.add_argument("-c", "--cd", default=Path("CDROM"), type=Path)
    parser.add_argument("-prob", "--project", default="katawa", type=str)
    parser.add_argument("-v", "--volume", default="KatawaShoujo", type=str)
    parser.add_argument("-pub", "--publisher", default="4LeafStudios", type=str)
    parser.add_argument(
        "-o", "--output", default=Path("CDROM/isoconfig.xml"), type=Path
    )

    args = parser.parse_args()

    return {
        "directory": args.cd,
        "projectName": args.project,
        "volume": args.volume,
        "publisher": args.publisher,
        "output": args.output
    }


def main():

    config = parse_cmd()

    rootDirectory = Path(config["directory"])
    print(rootDirectory)

    root = ET.Element(
        "iso_project",
        {
            "image_name": f"bin/{config['projectName']}.bin",
            "cue_sheet": f"bin/{config['projectName']}.cue",
        },
    )

    tracks = ET.SubElement(root, "track", {"type": "data"})

    identifiers = ET.SubElement(
        tracks,
        "identifiers",
        {
            "system": "PLAYSTATION",
            "application": "PLAYSTATION",
            "volume": config["volume"],
            "volume_set": config["volume"],
            "publisher": config["publisher"],
            "data_preparer": "MKPSXISO",
        },
    )

    directoryTree = ET.SubElement(tracks, "directory_tree")

    parse_folders(rootDirectory, rootDirectory, directoryTree)

    tree = ET.ElementTree(root)

    ET.indent(root, space="    ")

    tree.write(config["output"], xml_declaration=True, encoding="UTF-8")


if __name__ == "__main__":
    main()
