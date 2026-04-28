from pathlib import Path

for entry in Path(".").iterdir():
    if entry.is_dir():
        continue

    if "superclose" in str(entry):
        entry.rename(f"SCLOSE/{entry.name}")
    elif "close" in str(entry):
        entry.rename(f"CLOSE/{entry.name}")
    else:
        entry.rename(f"NORMAL/{entry.name}")