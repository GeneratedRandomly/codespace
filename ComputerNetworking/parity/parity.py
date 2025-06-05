import random


def generate_data(bits=64):  # 生成64位随机二进制数据
    return [random.randint(0, 1) for _ in range(bits)]


def add_parity_bits(data: list[int]):  # 每8位数据加1位奇偶校验，扩展为9位
    result: list[int] = []
    for i in range(0, len(data), 8):
        byte = data[i : i + 8]  # 取8位
        parity = sum(byte) % 2  # 计算偶校验位
        result.extend(byte + [parity])  # 添加校验位
    return result


def crc8(D: list[int], G=0x107, init=0):  # CRC-8 校验计算，G=100000111
    crc = init
    for bit in D:
        crc = ((crc << 1) | bit) & 0x1FF  # 左移一位并加上新数据位，保持9位
        if crc & 0x100:  # 如果最高位为1
            crc ^= G  # 与多项式异或
    return crc & 0xFF  # 返回低8位作为CRC校验码


def inject_errors(
    data: list[int], error_rate: float
):  # 按指定错误率随机翻转比特，返回新数据和错误位置
    corrupted = data[:]
    error_positions: list[int] = []
    for i in range(len(corrupted)):
        if random.random() < error_rate:  # 按概率翻转
            corrupted[i] ^= 1
            error_positions.append(i)  # 记录错误位置
    return corrupted, error_positions


def check_parity(data: list[int]):  # 检查每9位单元的奇偶校验位，返回每字节校验状态
    status: list[bool] = []
    for i in range(0, len(data), 9):
        byte = data[i : i + 9]
        if len(byte) < 9:
            continue
        parity = sum(byte[:8]) % 2  # 重新计算前8位的奇偶校验
        status.append(parity == byte[8])  # 校验是否正确
    return status


def origin_main():
    data = generate_data()  # 生成原始数据
    print("Original 64-bit data: ", "".join(map(str, data)))
    data_with_parity = add_parity_bits(data)  # 加入奇偶校验
    crc = crc8(data_with_parity)  # 计算CRC校验码
    print("CRC-8 checksum: {:08b}".format(crc))

    # 信道仿真：按输入错误率随机翻转比特
    error_rate = float(input("Enter bit error rate (e.g., 0.05): "))
    transmitted = data_with_parity + [int(x) for x in f"{crc:08b}"]  # 拼接数据和CRC
    print("Transmitted data with parity and CRC: ", "".join(map(str, transmitted)))

    received, _ = inject_errors(transmitted, error_rate)  # 注入错误
    print("Received raw data (with errors): ", "".join(map(str, received)))

    # 接收端校验
    received_data = received[:-8]  # 提取接收到的数据部分
    received_crc = int("".join(map(str, received[-8:])), 2)  # 提取接收到的CRC
    parity_status = check_parity(received_data)  # 检查每字节奇偶校验
    print(
        "Parity check status of each byte (Correct / Incorrect): ",
        ["Correct" if s else "Incorrect" for s in parity_status],
    )
    crc_check = crc8(received_data)  # 重新计算CRC
    print(
        "CRC check result (Passed / Failed): ",
        "Passed" if crc_check == received_crc else "Failed",
    )

    # 错误分析：CRC通过但实际数据不一致
    undetected_error = (crc_check == received_crc and all(parity_status)) and (
        not transmitted == received
    )
    print("Whether any undetected error exists: ", "Yes" if undetected_error else "No")


def main():
    error = [0.01, 0.02, 0.05, 0.1, 0.2, 0.5]
    for error_rate in error:
        ave = 0
        for _ in range(100):
            cnt = 0
            while True:
                cnt += 1
                data = generate_data()
                data_with_parity = add_parity_bits(data)
                crc = crc8(data_with_parity)
                transmitted = data_with_parity + [int(x) for x in f"{crc:08b}"]
                received, _ = inject_errors(transmitted, error_rate)
                received_data = received[:-8]
                received_crc = int("".join(map(str, received[-8:])), 2)
                parity_status = check_parity(received_data)
                crc_check = crc8(received_data)
                undetected_error = (
                    crc_check == received_crc and all(parity_status)
                ) and (transmitted != received)
                if undetected_error:
                    ave += cnt
                    break
        ave /= 100
        print(f"Error rate: {error_rate}, Average undetected error count: {ave:.2f}")


if __name__ == "__main__":
    main()
