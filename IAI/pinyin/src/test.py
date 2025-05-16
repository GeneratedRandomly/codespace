from pathlib import Path
import numpy as np
from collections import Counter

one_path = Path(__file__).parent.parent / "frequency" / "one.npz"
one = np.load(one_path, allow_pickle=True)["one"].item()
one = Counter({k: v for k, v in one.items() if v < 50})


def calculate_accuracy(output_lines: list[str], answer_lines: list[str]):
    total_lines = len(answer_lines)
    correct_lines = sum(1 for o, a in zip(output_lines, answer_lines) if o == a)
    correct_chars = sum(
        sum(1 for oc, ac in zip(o, a) if oc == ac)
        for o, a in zip(output_lines, answer_lines)
    )

    total_chars = sum(len(a) for a in answer_lines)
    line_accuracy = correct_lines / total_lines
    char_accuracy = correct_chars / total_chars

    # 统计 output_lines 和 answer_lines 中在 one 中的字符数量
    output_one_chars = sum(1 for line in output_lines for char in line if char in one)
    answer_one_chars = sum(1 for line in answer_lines for char in line if char in one)

    return line_accuracy, char_accuracy, output_one_chars, answer_one_chars


def test():
    output_path = Path(__file__).parent.parent / "data" / "output.txt"
    answer_path = Path(__file__).parent.parent / "data" / "answer.txt"
    with open(output_path, "r") as output_file, open(answer_path, "r") as answer_file:
        output_lines = output_file.readlines()
        answer_lines = answer_file.readlines()
    return calculate_accuracy(output_lines, answer_lines)
