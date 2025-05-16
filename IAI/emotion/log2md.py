import re


def extract_results(log_file, var):
    results = []
    align_length = None
    current_results = None
    with open(log_file, "r") as file:
        lines = file.readlines()

    # 动态生成正则表达式，提取对齐长度（Align Length 或其他变量）
    align_pattern = re.compile(rf"{var}: ([\w\s\d.]+)")  # 支持文字、数字、空格
    epoch_pattern = re.compile(
        r"Epoch \[(\d+)/\1\], Train Loss: [\d.]+, Train Acc: ([\d.]+), Train F1: ([\d.]+), "
        r"Val Loss: [\d.]+, Val Acc: ([\d.]+), Val F1: ([\d.]+), "
        r"Test Loss: [\d.]+, Test Acc: ([\d.]+), Test F1: ([\d.]+)"
    )

    for line in lines:
        # 检查是否是新的 Align Length 部分
        align_match = align_pattern.search(line)
        if align_match:
            # 如果有当前结果，保存最后一行
            if current_results:
                results.append(current_results)
            align_length = align_match.group(1).strip()  # 保留文字或数字，去除多余空格
            current_results = None  # 重置当前结果

        # 匹配 Epoch [a/a] 格式的最后一行
        epoch_match = epoch_pattern.search(line)
        if epoch_match:
            current_results = {
                filename: align_length,
                "Train Acc": float(epoch_match.group(2)),
                "Train F1": float(epoch_match.group(3)),
                "Val Acc": float(epoch_match.group(4)),
                "Val F1": float(epoch_match.group(5)),
                "Test Acc": float(epoch_match.group(6)),
                "Test F1": float(epoch_match.group(7)),
            }

    # 保存最后一个部分的结果
    if current_results:
        results.append(current_results)

    return results


def generate_markdown_table(results):
    # Markdown 表格头
    markdown = f"| {filename} | Train Acc | Train F1 | Val Acc | Val F1 | Test Acc | Test F1 |\n"
    markdown += (
        "|-------|-----------|----------|---------|--------|----------|---------|\n"
    )

    # 填充表格内容
    for result in results:
        markdown += f"| {result[filename]} | {result['Train Acc']:.4f} | {result['Train F1']:.4f} | {result['Val Acc']:.4f} | {result['Val F1']:.4f} | {result['Test Acc']:.4f} | {result['Test F1']:.4f} |\n"

    return markdown


filename = "model"
var = "Neural Name"

if __name__ == "__main__":
    # 指定变量字符串
    log_file = f"/root/codespace/IAI/emotion/log/{filename}.log"  # 日志文件路径
    results = extract_results(log_file, var)
    markdown_table = generate_markdown_table(results)
    print(markdown_table)
