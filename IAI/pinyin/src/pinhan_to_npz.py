from pathlib import Path
import numpy as np


def pinhan_to_npz():
    # 设置拼音汉字表的文件路径
    pinyin_dir = Path(__file__).parent.parent / "data" / "拼音汉字表.txt"
    pinyin_hanzi_dict = {}
    hanzi_pinyin_dict: dict[list[str]] = {}
    with open(pinyin_dir, "r", encoding="gbk") as f:
        for line in f:
            # 去除行首尾空白字符并分割每行，获取拼音和汉字列表
            parts = line.strip().split()
            pinyin = parts[0]  # 拼音
            hanzi_list = parts[1:]  # 汉字列表
            # 将拼音和汉字列表添加到 pinyin_hanzi_dict 中
            # {"pinyin": ["hanzi1", "hanzi2", ...]}
            pinyin_hanzi_dict[pinyin] = hanzi_list
            # 将汉字和拼音添加到 hanzi_pinyin_dict 中
            # {"hanzi": ["pinyin1", "pinyin2", ...]}
            for hanzi in hanzi_list:
                if hanzi not in hanzi_pinyin_dict:
                    hanzi_pinyin_dict[hanzi] = []
                hanzi_pinyin_dict[hanzi].append(pinyin)
    save_dir = Path(__file__).parent.parent / "pin_han_table"
    save_dir.mkdir(parents=True, exist_ok=True)
    # 保存字典为 .npz 文件
    np.savez(save_dir / "pinyin_hanzi_table.npz", pinyin_hanzi_dict=pinyin_hanzi_dict)
    np.savez(save_dir / "hanzi_pinyin_table.npz", hanzi_pinyin_dict=hanzi_pinyin_dict)
