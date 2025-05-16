import math
from pathlib import Path
import numpy as np

frequency_path = Path(__file__).parent.parent / "frequency"
# 单个汉字在语料库中出现次数
one = np.load(frequency_path / "one.npz", allow_pickle=True)["one"].item()
# 汉字二元组在语料库中出现次数
two = np.load(frequency_path / "two.npz", allow_pickle=True)["two"].item()
# 汉字三元组在语料库中出现次数
three = np.load(frequency_path / "three.npz", allow_pickle=True)["three"].item()

pin_han_path = Path(__file__).parent.parent / "pin_han_table"
pin_han_table = np.load(pin_han_path / "pinyin_hanzi_table.npz", allow_pickle=True)[
    "pinyin_hanzi_dict"
].item()

# 二元、三元模型占比
two_prop = 0
three_prop = 0

possible_hanzi = []


def calcu_prob(hanzi_str: str) -> float:  # 未出现过的汉字，惩罚
    length = len(hanzi_str)
    if length == 1:
        if one[hanzi_str] != 0:
            return math.log(one[hanzi_str])
    elif length == 2:
        if two[hanzi_str] != 0 and one[hanzi_str[0]] != 0 and one[hanzi_str[1]] != 0:
            return two_prop * math.log(
                two[hanzi_str] / one[hanzi_str[0]] + (1 - two_prop) * one[hanzi_str[1]]
            )
    else:
        if (
            three[hanzi_str] != 0
            and two[hanzi_str[1:3]] != 0
            and two[hanzi_str[:2]] != 0
            and one[hanzi_str[0]] != 0
            and one[hanzi_str[1]] != 0
            and one[hanzi_str[2]] != 0
        ):
            return math.log(
                three_prop * three[hanzi_str] / two[hanzi_str[:2]]
                + (1 - three_prop)
                * (
                    two_prop * two[hanzi_str[1:3]] / one[hanzi_str[1]]
                    + (1 - two_prop) * one[hanzi_str[2]]
                )
            )
    return -50


def set_possible_hanzi(pinyin_list: list[str]) -> None:
    global possible_hanzi
    possible_hanzi = [pin_han_table.get(pinyin, []) for pinyin in pinyin_list]


def viterbi_impl(
    pinyin_list: list[str], use_three: bool = False
) -> tuple[dict[str, float], dict[str, list[str]]]:
    # combination of possible hanzi for every pinyin
    prev_hanzi_prob = {}
    curr_hanzi_prob = {}
    path = {}

    # initialize the first layer
    for hanzi0 in possible_hanzi[0]:
        curr_hanzi_prob[hanzi0] = calcu_prob(hanzi0)
        path[hanzi0] = [hanzi0]
    # start viterbi algorithm
    for layer in range(1, len(pinyin_list)):
        prev_hanzi_prob = curr_hanzi_prob
        curr_hanzi_prob = {}
        newpath = {}
        curr_candidates = possible_hanzi[layer]
        prev_candidates = possible_hanzi[layer - 1]

        for curr_hanzi in curr_candidates:
            best_prob = -math.inf
            best_prev_hanzi = None
            for prev_hanzi in prev_candidates:
                if layer > 1 and use_three:
                    prev_prev_hanzi = path[prev_hanzi][-2]
                    hanzi_pair = f"{prev_prev_hanzi}{prev_hanzi}{curr_hanzi}"
                else:
                    hanzi_pair = f"{prev_hanzi}{curr_hanzi}"
                path_prob = prev_hanzi_prob[prev_hanzi] + calcu_prob(hanzi_pair)
                if path_prob > best_prob:
                    best_prob = path_prob
                    best_prev_hanzi = prev_hanzi
            curr_hanzi_prob[curr_hanzi] = best_prob
            newpath[curr_hanzi] = path[best_prev_hanzi] + [curr_hanzi]
        path = newpath
    return curr_hanzi_prob, path


def viterbi(
    pinyin_list: list[str],
    use_three: bool = False,
    three_proportion: float = 0.9,
    two_proportion: float = 1,
) -> str:
    global two_prop, three_prop
    two_prop = two_proportion
    three_prop = three_proportion

    set_possible_hanzi(pinyin_list)

    (curr_hanzi_prob, path) = viterbi_impl(pinyin_list, use_three=use_three)
    final_hanzi = max(curr_hanzi_prob, key=curr_hanzi_prob.get)
    return "".join(path[final_hanzi])
