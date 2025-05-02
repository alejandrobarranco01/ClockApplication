SEGMENTS = ["A SEGMENT", "B SEGMENT", "C SEGMENT", "D SEGMENT", "E SEGMENT", "F SEGMENT", "G SEGMENT"]

ascii_to_seven_segment_map = {
    '0000': '0111111',  # 0
    '0001': '0000110',  # 1
    '0010': '1011011',  # 2
    '0011': '1001111',  # 3
    '0100': '1100110',  # 4
    '0101': '1101101',  # 5
    '0110': '1111101',  # 6
    '0111': '0000111',  # 7
    '1000': '1111111',  # 8
    '1001': '1101111',  # 9
    '1111': '0000000'   # OFF
}

def filter_keys_by_bit(mapping, index):
    print(f"{SEGMENTS[index]} lit with BCD values: \t", end="")
    matching_keys = []
    for key in mapping.keys():
        if mapping[key][index] == '1':
            matching_keys.append(key)
            print(f"{int(key, 2)},", end=" ")
    print()

# Iterate from segment A (index 0) to G (index 6)
for x in range(7):
    filter_keys_by_bit(ascii_to_seven_segment_map, x)