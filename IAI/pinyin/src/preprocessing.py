from .count import save_statistic
from .pinhan_to_npz import pinhan_to_npz
from .corpus_washer import washer_main
from pathlib import Path

def prepare(expand: bool = False):
    """
    预处理。
    参数:
        expand (bool): 如果为 True，则统计三字符组合的频率；否则只统计单字符和双字符组合的频率。
    """
    pin_han_table_path = Path(__file__).parent.parent / "pin_han_table"
    washed_path = Path(__file__).parent.parent / "washed"
    
    if not pin_han_table_path.exists():
        pinhan_to_npz()
    if not washed_path.exists():
        washer_main()
    save_statistic(expand=expand)