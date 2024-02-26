def get_data(filename: str) -> bytes:
    with open(filename, 'rb') as f:
        data = f.read()
    return data


def bits_to_byte(bits: str) -> int:
    ans = 0
    for power in range(8):
        ans += int(bits[power]) * 2 ** (7 - power)
    return ans


def encode_lsb(*, image: str, file_to_encode: str, image_encoded: str) -> str:
    image_data = get_data(image)
    image_data = bytearray(image_data)
    file_data = get_data(file_to_encode)
    offset = 54
    byte_numbers = 4
    curr = 0
    # print(file_data)
    if len(image_data) < len(file_data) * 4 + offset:
        return 'image is less'
    for byte in file_data:
        bits = f'{int(str(byte), 10):08b}'
        # print(bits)
        # print(offset + curr * byte_numbers, offset + curr * byte_numbers + 4)
        b1, b2, b3, b4 = image_data[offset + curr * byte_numbers: offset + curr * byte_numbers + 4]
        bits1, bits2, bits3, bits4 = [f'{int(str(i), 10):08b}' for i in [b1, b2, b3, b4]]
        bits11 = bits1[0:6] + bits[0:2]
        bits22 = bits2[0:6] + bits[2:4]
        bits33 = bits3[0:6] + bits[4:6]
        bits44 = bits4[0:6] + bits[6:8]
        # print(bits1, bits2, bits3, bits4)
        # print(bits11, bits22, bits33, bits44)
        image_data[offset + curr * byte_numbers] = bits_to_byte(bits11)
        image_data[offset + curr * byte_numbers + 1] = bits_to_byte(bits22)
        image_data[offset + curr * byte_numbers + 2] = bits_to_byte(bits33)
        image_data[offset + curr * byte_numbers + 3] = bits_to_byte(bits44)
        curr += 1
    with open(image_encoded, 'wb') as f:
        f.write(image_data)
    return 'success'


def decode_lsb(*, file_to_decode: str, file_decoded: str) -> str:
    d = get_data(file_to_decode)[54:]
    new = ''
    for byte in d:
        a = f'{int(str(byte), 10):08b}'
        new += a[6:8]
    new_data = bytearray()
    for i in range(len(new) // 8):
        new_data.append(bits_to_byte(new[8 * i:8 * (i + 1)]))
    with open(file_decoded, 'wb') as file:
        file.write(new_data)
    return 'success'


if __name__ == '__main__':
    #encode_lsb(image='2.bmp', file_to_encode='file.txt', image_encoded='new_image.bmp')
    decode_lsb(file_to_decode='new_image.bmp', file_decoded='newfile.txt')