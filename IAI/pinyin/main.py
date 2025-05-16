from src.preprocessing import prepare
import argparse


def main():
    parser = argparse.ArgumentParser(prog="typein")
    parser.add_argument(
        "-e", "--expand", action="store_true", required=False, help="使用三元模型"
    )
    parser.add_argument(
        "-2",
        "--two_proportion",
        type=float,
        default=1,
        required=False,
        help="二元模型比例",
    )
    parser.add_argument(
        "-3",
        "--three_proportion",
        type=float,
        default=1,
        required=False,
        help="三元模型比例",
    )
    parser.add_argument(
        "-a", "--calcu_accuracy", action="store_true", required=False, help="计算准确率"
    )
    args = parser.parse_args()
    expand: bool = args.expand
    two_proportion: float = args.two_proportion
    if two_proportion < 0:
        two_proportion = 0
    if two_proportion > 1:
        two_proportion = 1
    three_proportion: float = args.three_proportion
    if three_proportion < 0:
        three_proportion = 0
    if three_proportion > 1:
        three_proportion = 1
    calcu_accuracy: bool = args.calcu_accuracy

    prepare(expand=True)

    from src.viterbi import viterbi

    pinyin_list = input().strip().split()
    try:
        while True:
            result = viterbi(
                pinyin_list=pinyin_list,
                use_three=expand,
                two_proportion=two_proportion,
                three_proportion=three_proportion,
            )
            print(result)
            pinyin_list = input().strip().split()
    except EOFError:
        pass

    if calcu_accuracy:
        from src.test import test

        line, char, out, ans = test()
        print(
            f"line_accuracy={line:.4f}, char_accuracy={char:.4f}, output_rare_chars={out}, answer_rare_chars={ans}"
        )


if __name__ == "__main__":
    main()
