"""
corpus_washer.py

这个模块包含用于处理语料库的函数，包括分割句子和清洗文件。

函数:
    split_sentence(sentence: str) -> dict[str, None]:
        将句子按非中文字符分割，并返回一个包含分割结果的字典。

    file_washer(path: Path):
        读取文件，分割句子，保存语料文件的句子。

    washer_main():
        读取文件夹中的所有文件，分割句子，并调用 file_washer 函数处理每个文件。
"""

import json
import re
from pathlib import Path
import numpy as np
from tqdm import tqdm


def split_sentence(sentence: str):
    # 用非中文字符分割句子
    parts = re.split(r"[^\u4e00-\u9fa5]+", sentence)
    parts_dict = {part: None for part in parts if len(part) > 1}
    return parts_dict


def file_washer(path: Path):
    if path.name == "README.txt":
        return
    # 读取文件并分割句子
    sentence_list: dict[str] = {}
    with open(path, "r", encoding="gbk") as f:
        for line in f.readlines():
            data = json.loads(line)
            sentence_list.update(split_sentence(data["title"]))
            sentence_list.update(split_sentence(data["html"]))
    # 获取上级文件夹路径，如果不存在则创建
    parent_folder = path.parent.parent.parent / "washed"
    parent_folder.mkdir(parents=True, exist_ok=True)
    # 构建保存文件的路径
    save_path = parent_folder / f"{path.stem}.npz"
    # 保存处理后的文件
    np.savez(save_path, string=np.array(list(sentence_list.keys()), dtype=object))


def washer_main():
    # 读取语料库文件夹，处理每个文件
    path = Path(__file__).parent.parent / "corpus" / "sina_news_gbk"
    for file in tqdm(path.iterdir(), desc="Washing corpus"):
        file_washer(file)


washer_main()
