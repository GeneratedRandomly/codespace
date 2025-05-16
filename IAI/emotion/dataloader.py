import torch
from torch.utils.data import DataLoader, TensorDataset
from load import getCorpus


def getDataloader(batch_size: int = 50, alignLen: int = 50):
    train_path = "./Dataset/train.txt"
    valid_path = "./Dataset/validation.txt"
    test_path = "./Dataset/test.txt"

    train_contents, train_labels = getCorpus(train_path, alignLen)
    val_contents, val_labels = getCorpus(valid_path, alignLen)
    test_contents, test_labels = getCorpus(test_path, alignLen)

    train_dataset = TensorDataset(
        torch.from_numpy(train_contents).type(torch.int32),
        torch.from_numpy(train_labels).type(torch.long),
    )
    val_dataset = TensorDataset(
        torch.from_numpy(val_contents).type(torch.int32),
        torch.from_numpy(val_labels).type(torch.long),
    )
    test_dataset = TensorDataset(
        torch.from_numpy(test_contents).type(torch.int32),
        torch.from_numpy(test_labels).type(torch.long),
    )

    train_dataloader = DataLoader(train_dataset, batch_size, True, num_workers=2)
    val_dataloader = DataLoader(val_dataset, batch_size, True, num_workers=2)
    test_dataloader = DataLoader(test_dataset, batch_size, True, num_workers=2)

    return train_dataloader, val_dataloader, test_dataloader
