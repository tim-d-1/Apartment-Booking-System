import os
import re
import json
import argparse
import time

# Try to import deep_translator
try:
    from deep_translator import GoogleTranslator
    HAS_TRANSLATOR = True
except ImportError:
    HAS_TRANSLATOR = False

# Configuration
EXTENSIONS = {'.cpp', '.c', '.h', '.hpp', '.cc', '.cxx'}
IGNORE_DIRS = {'.git', 'build', 'bin', 'obj', 'node_modules', 'debug', 'release'}

# ---------------------------------------------------------------------------
# ROBUST C++ TOKENIZER REGEX
# This regex matches specific C++ structures in priority order.
# 1. Block Comments (/* ... */)
# 2. Line Comments (// ...)
# 3. Includes (#include ...)
# 4. Char Literals ('x' or '\n') - CRITICAL: Prevents ' from starting a capture
# 5. String Literals ("text") - This is what we want
# 6. Any other character (Code)
# ---------------------------------------------------------------------------
TOKEN_SPEC = [
    ('BLOCK_COMMENT', r'/\*[\s\S]*?\*/'),      # Match /* ... */ (crosses lines)
    ('LINE_COMMENT',  r'//.*'),                # Match // until end of line
    ('INCLUDE',       r'#include.*'),          # Match #include lines
    ('CHAR',          r"'(\\.|[^\\'])*'"),     # Match 'a', '\n', 'b'
    ('STRING',        r'"(\\.|[^\\"])*"'),     # Match "string"
    ('OTHER',         r'[\s\S]'),              # Match any other char (code)
]

# Compile the master regex
TOKEN_REGEX = re.compile('|'.join(f'(?P<{name}>{pattern})' for name, pattern in TOKEN_SPEC))

def get_files(root_dir):
    """Recursively yield file paths matching extensions."""
    for dirpath, dirnames, filenames in os.walk(root_dir):
        dirnames[:] = [d for d in dirnames if d.lower() not in IGNORE_DIRS]
        for filename in filenames:
            if os.path.splitext(filename)[1].lower() in EXTENSIONS:
                yield os.path.join(dirpath, filename)

def extract_strings(root_dir, output_json):
    print(f"Scanning {root_dir} using Logic V2 (Tokenizer)...")
    
    found_strings = set()
    
    for file_path in get_files(root_dir):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                
            # Iterate over all tokens found in the file
            for match in TOKEN_REGEX.finditer(content):
                kind = match.lastgroup
                value = match.group()
                
                if kind == 'STRING':
                    # value is "text", we strip quotes to store the inner text
                    inner_text = value[1:-1]
                    
                    # Filter out empty strings or technical strings mostly symbols
                    if len(inner_text) > 1 and any(c.isalpha() for c in inner_text):
                        found_strings.add(inner_text)
                        
        except Exception as e:
            print(f"Skipping {file_path}: {e}")

    print(f"Found {len(found_strings)} unique strings.")
    
    # Translation Logic
    translation_map = {}
    if HAS_TRANSLATOR:
        translator = GoogleTranslator(source='en', target='uk')
        print("Starting translation...")
    else:
        print("WARNING: 'deep-translator' not found. Generating empty JSON.")

    total = len(found_strings)
    for i, s in enumerate(found_strings):
        translated_text = s
        if HAS_TRANSLATOR:
            try:
                translated_text = translator.translate(s)
                # Rate limit
                time.sleep(0.2) 
                print(f"[{i+1}/{total}] '{s}' -> '{translated_text}'")
            except Exception as e:
                print(f"Error translating '{s}': {e}")
        
        translation_map[s] = translated_text

    with open(output_json, 'w', encoding='utf-8') as f:
        json.dump(translation_map, f, indent=4, ensure_ascii=False)
    
    print(f"\nSaved to {output_json}. Please REVIEW before replacing.")

def replace_strings(root_dir, input_json):
    print(f"Loading translations from {input_json}...")
    
    try:
        with open(input_json, 'r', encoding='utf-8') as f:
            translation_map = json.load(f)
    except FileNotFoundError:
        print("JSON file not found.")
        return

    count = 0
    
    for file_path in get_files(root_dir):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            new_content = []
            last_pos = 0
            modified = False

            # We rebuild the file token by token
            for match in TOKEN_REGEX.finditer(content):
                kind = match.lastgroup
                value = match.group()
                
                if kind == 'STRING':
                    original_inner = value[1:-1] # strip quotes
                    
                    if original_inner in translation_map:
                        trans = translation_map[original_inner]
                        # Reconstruct string with quotes
                        # Note: simple replacement. Be careful if original had escaped quotes.
                        new_value = f'"{trans}"'
                        new_content.append(new_value)
                        if new_value != value:
                            modified = True
                    else:
                        new_content.append(value)
                else:
                    # For comments, code, includes, chars -> Keep exactly as is
                    new_content.append(value)

            if modified:
                final_output = "".join(new_content)
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(final_output)
                count += 1
                print(f"Updated: {file_path}")

        except Exception as e:
            print(f"Error processing {file_path}: {e}")

    print(f"\nReplacement complete! Modified {count} files.")

def main():
    parser = argparse.ArgumentParser(description="C++ Project Localizer V2 (Safe Tokenizer)")
    parser.add_argument('action', choices=['extract', 'replace'], help="Action to perform")
    parser.add_argument('--path', default='.', help="Path to project")
    parser.add_argument('--json', default='translations.json', help="Path to JSON")
    
    args = parser.parse_args()

    if args.action == 'extract':
        extract_strings(args.path, args.json)
    elif args.action == 'replace':
        print("⚠️  WARNING: This will overwrite files.")
        confirm = input("Type 'yes' to continue: ")
        if confirm.lower() == 'yes':
            replace_strings(args.path, args.json)

if __name__ == "__main__":
    main()