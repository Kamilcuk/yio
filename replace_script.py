import os
import re
import sys

def process_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    original = content
    
    content = content.replace('π', '')
    content = content.replace('Ω', '')
    
    # TC('c') -> 'c', TC("s") -> "s"
    content = re.sub(r'\bTC\((.*?)\)', r'\1', content)
    
    # TCHAR -> char
    content = re.sub(r'\bTCHAR\b', 'char', content)
    # TINT -> int
    content = re.sub(r'\bTINT\b', 'int', content)
    # TEOF -> EOF
    content = re.sub(r'\bTEOF\b', 'EOF', content)
    # TPRI -> "s"
    content = re.sub(r'\bTPRI\b', '"s"', content)
    
    # TMODEX, TMODE, TMODEN -> 1
    content = re.sub(r'\bTMODEX\b', '1', content)
    content = re.sub(r'\bTMODE\b', '1', content)
    content = re.sub(r'\bTMODEN\b', '1', content)
    
    # Function replacements
    content = re.sub(r'\bTFPRINTF\((.*?)\)', r'fprintf(\1)', content)
    content = re.sub(r'\bTISDIGIT\((.*?)\)', r'isdigit((unsigned char)(\1))', content)
    content = re.sub(r'\bTISXDIGIT\((.*?)\)', r'isxdigit((unsigned char)(\1))', content)
    content = re.sub(r'\bTSTRCHR\((.*?)\)', r'strchr(\1)', content)
    content = re.sub(r'\bTSTRCMP\((.*?)\)', r'strcmp(\1)', content)
    content = re.sub(r'\bTSTRLEN\((.*?)\)', r'strlen(\1)', content)

    if content != original:
        with open(filepath, 'w') as f:
            f.write(content)
        print(f"Modified {filepath}")

for d in sys.argv[1:]:
    if os.path.isfile(d):
        process_file(d)
    else:
        for root, _, files in os.walk(d):
            for file in files:
                if file.endswith(('.c', '.h', '.pp')):
                    process_file(os.path.join(root, file))
