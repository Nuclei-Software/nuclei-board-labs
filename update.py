#!/bin/env python
import os
import yaml
from datetime import datetime

def find_npk_files(folder='.'):
    npk_files = []
    for root, dirs, files in os.walk(folder):
        for file in files:
            if file.endswith('.yml') and file.lower() == 'npk.yml':
                npk_files.append(os.path.join(root, file))
    return npk_files

def generate_markdown_table(npk_files):

    table_prefix = "\n"
    # Get the current date and time
    current_datetime = datetime.now()

    # Format the date and time as a string
    formatted_datetime = current_datetime.strftime("%Y-%m-%d %H:%M:%S")
    table_prefix += "> Generated by `python3 update.py` @ %s\n\n" % (formatted_datetime)

    table_header = "| Application | Board/SOC | Description |\n| --- | --- | --- |\n"
    table_content = ""

    for npk_file in npk_files:
        with open(npk_file, 'r') as f:
            npk_data = yaml.safe_load(f)
            if npk_data.get('type') == 'app':
                app = os.path.dirname(npk_file)
                board_soc = app.split(os.sep)[1]
                description = npk_data.get('description', '')
                table_content += f"| [{app}]({app}) | {board_soc} | {description} |\n"

    return table_prefix + table_header + table_content


def update_readme(file_path, table_content):
    with open(file_path, 'r') as f:
        readme_content = f.read()

    start_marker = '## Board Labs Listing'
    end_marker = '## Use it in Nuclei Studio'

    start_idx = readme_content.find(start_marker) + len(start_marker)
    end_idx = readme_content.find(end_marker)

    updated_readme = (
        readme_content[:start_idx] +
        '\n' + table_content + '\n' +
        readme_content[end_idx:]
    )

    with open(file_path, 'w') as f:
        f.write(updated_readme)

if __name__ == "__main__":
    npk_files = find_npk_files()
    table_content = generate_markdown_table(npk_files)

    readme_file_path = 'README.md'  # Replace with the actual path to your README.md file
    update_readme(readme_file_path, table_content)

    print("Markdown table has been generated and inserted into the README.md file.")

