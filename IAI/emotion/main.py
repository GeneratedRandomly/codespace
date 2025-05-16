import torch
import torch.nn as nn
import torch.optim as optim
import logging
import sys
from torch.utils.data import DataLoader
from tqdm import tqdm
from dataloader import getDataloader
from config import parser_data, device
from sklearn.metrics import f1_score


def train():
    model.train()
    train_loss = 0.0
    correct, total = 0, 0
    all_labels = []
    all_predictions = []
    for texts, labels in train_loader:  # a batch of data
        texts: torch.Tensor = texts.to(device)
        labels: torch.Tensor = labels.to(device)
        optimizer.zero_grad()
        output: torch.Tensor = model(texts)
        loss: torch.Tensor = criterion(output, labels)
        loss.backward()
        optimizer.step()
        train_loss += loss.item()
        predicted = output.argmax(1)
        correct += (predicted == labels).float().sum().item()
        total += labels.size(0)
        all_labels.extend(labels.cpu().tolist())
        all_predictions.extend(predicted.cpu().tolist())
    train_loss /= len(train_loader)
    train_acc = correct / total
    scheduler.step()
    train_f1 = f1_score(all_labels, all_predictions, average="binary")
    return train_loss, train_acc, train_f1


def evaluate(loader: DataLoader):
    model.eval()
    eval_loss = 0.0
    correct, total = 0, 0
    all_labels = []
    all_predictions = []
    with torch.no_grad():
        for texts, labels in loader:
            texts: torch.Tensor = texts.to(device)
            labels: torch.Tensor = labels.to(device)
            output: torch.Tensor = model(texts)
            loss: torch.Tensor = criterion(output, labels)
            eval_loss += loss.item()
            predicted = output.argmax(1)
            correct += (predicted == labels).float().sum().item()
            total += labels.size(0)
            all_labels.extend(labels.cpu().tolist())
            all_predictions.extend(predicted.cpu().tolist())
    eval_loss /= len(loader)
    eval_acc = correct / total
    eval_f1 = f1_score(all_labels, all_predictions, average="binary")
    return eval_loss, eval_acc, eval_f1


logging.basicConfig(
    filename="main.log",
    filemode="a",
    format="%(message)s",
    level=logging.INFO,
)

learning_rate, epochs, alignLen, batch_size, neural_name, model, config = parser_data()
logging.info(
    f"Learning rate: {learning_rate}, Epochs: {epochs}, Align Length: {alignLen}, "
    f"Batch Size: {batch_size}, Neural Name: {neural_name}, "
    f"Hidden Size: {config.hidden_size}, Number of Layers: {config.num_layers}, "
    f"Dropout: {config.dropout}, Number of Filters: {config.num_filters}, "
    f"Update Word2Vec: {config.update_w2v}, Internal: {config.internal}"
)
train_loader, val_loader, test_loader = getDataloader(batch_size, alignLen)
criterion = nn.CrossEntropyLoss()
optimizer = optim.AdamW(model.parameters(), learning_rate, weight_decay=1e-5)
scheduler = optim.lr_scheduler.StepLR(optimizer, 5)

for epoch in tqdm(range(epochs), file=sys.stdout):
    train_loss, train_accuracy, train_f1 = train()
    val_loss, val_accuracy, val_f1 = evaluate(val_loader)
    test_loss, test_accuracy, test_f1 = evaluate(test_loader)
    logging.info(
        f"Epoch [{epoch + 1}/{epochs}], "
        f"Train Loss: {train_loss:.4f}, Train Acc: {train_accuracy:.4f}, Train F1: {train_f1:.4f}, "
        f"Val Loss: {val_loss:.4f}, Val Acc: {val_accuracy:.4f}, Val F1: {val_f1:.4f}, "
        f"Test Loss: {test_loss:.4f}, Test Acc: {test_accuracy:.4f}, Test F1: {test_f1:.4f}"
    )
logging.info("\n")
logging.shutdown()
