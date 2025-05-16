import numpy as np
from collections import Counter
from tqdm import tqdm
from pathlib import Path


def save_statistic(expand: bool = False):
    """
    统计句子中的字符和字符组合的频率，并保存到文件中。

    参数:
    expand (bool): 如果为 True，则统计三字符组合的频率；否则只统计单字符和双字符组合的频率。
    """
    # 读取处理后的文件，按月份
    month_list = ["04", "05", "06", "07", "08", "09", "10", "11"]

    save_path = Path(__file__).parent.parent / "frequency"
    save_path.mkdir(parents=True, exist_ok=True)
    one_path = save_path / "one.npz"
    two_path = save_path / "two.npz"
    three_path = save_path / "three.npz"

    if one_path.exists() and two_path.exists() and three_path.exists():
        return

    one = Counter()
    two = Counter()
    three = Counter()

    for month in tqdm(month_list, desc="Loading files"):
        # project/washed/2016-04.npz
        file_path = Path(__file__).parent.parent / "washed" / f"2016-{month}.npz"
        sentence_list = list(np.load(file_path, allow_pickle=True)["string"])

        if not one_path.exists():
            one += Counter("".join(sentence_list))
        if not two_path.exists():
            two_list = list()
            for sentence in tqdm(sentence_list, desc="Generating two list"):
                two_list += [sentence[i : i + 2] for i in range(0, len(sentence) - 1)]
            two += Counter(two_list)

        if not expand:
            continue
        if not three_path.exists():
            three_list = list()
            for sentence in tqdm(sentence_list, desc="Generating three list"):
                three_list += [sentence[i : i + 3] for i in range(0, len(sentence) - 2)]
            three += Counter(three_list)
    
    one = Counter({k: v for k, v in one.items() if v > 1})
    if not one_path.exists():
        np.savez(one_path, one=one)
    if not two_path.exists():
        np.savez(save_path / "two.npz", two=two)
    if expand and not three_path.exists():
        np.savez(save_path / "three.npz", three=three)
