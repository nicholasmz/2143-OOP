#!/usr/bin/env python3
"""
Simple build script using Python to avoid compiler installation issues.
This reads the JSON file and demonstrates the core functionality.
"""

import json
import sys
from pathlib import Path

def print_shape(shape_data, name):
    """Print a shape in grid format"""
    cells = shape_data.get('cells', [])
    
    if not cells:
        print(f"{name}: (empty shape)")
        return
    
    # Find bounds
    min_x = min(c['x'] for c in cells)
    max_x = max(c['x'] for c in cells)
    min_y = min(c['y'] for c in cells)
    max_y = max(c['y'] for c in cells)
    
    width = max_x - min_x + 1
    height = max_y - min_y + 1
    
    # Create grid
    grid = [['.' for _ in range(width)] for _ in range(height)]
    
    for cell in cells:
        gx = cell['x'] - min_x
        gy = cell['y'] - min_y
        if 0 <= gy < height and 0 <= gx < width:
            grid[gy][gx] = '#'
    
    size = shape_data.get('size', {})
    print(f"\n{name} ({size.get('w')}x{size.get('h')})")
    for row in grid:
        print(''.join(row))

def main():
    # Load JSON
    try:
        with open('shapes.json', 'r') as f:
            data = json.load(f)
    except FileNotFoundError:
        print("Error: Could not open shapes.json")
        sys.exit(1)
    
    if 'shapes' not in data:
        print("Error: JSON missing 'shapes' key")
        sys.exit(1)
    
    shapes_data = data['shapes']
    print("Available shapes:")
    for key in shapes_data.keys():
        print(f" - {key}")
    
    print(f"\nTotal shapes loaded: {len(shapes_data)}")
    
    # Get user choice
    choice = input("\nEnter shape name: ").strip()
    
    if choice not in shapes_data:
        print("Shape not found.")
        sys.exit(1)
    
    # Print selected shape
    print_shape(shapes_data[choice], choice)

if __name__ == '__main__':
    main()
