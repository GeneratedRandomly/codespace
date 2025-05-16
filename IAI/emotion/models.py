import torch
import torch.nn as nn
from load import getId2Vec


class Config:
    vocab_size = 59290  # rows of word2vec = len(word2id) + 1
    embedding_dim = 50  # dimension of word2vec
    output_dim = 2  # number of classes (2)
    pretrained_embed = torch.from_numpy(getId2Vec())  # pre-trained word2vec
    filter_sizes = [3, 5, 7]  # how many words filtered once

    hidden_size = 128  # LSTM hidden size
    num_layers = 2  # LSTM layers
    dropout = 0.5  # dropout rate
    num_filters = 100  # number of filters (CNN)
    update_w2v = False  # whether to update word2vec during training
    internal = 32


class CNN(nn.Module):
    def __init__(self, config: Config):
        super(CNN, self).__init__()

        self.embedding = nn.Embedding(config.vocab_size, config.embedding_dim)
        self.embedding.weight.data.copy_(config.pretrained_embed)
        self.embedding.weight.requires_grad = config.update_w2v

        self.convs = nn.ModuleList(
            [
                nn.Conv2d(
                    in_channels=1,
                    out_channels=config.num_filters,
                    kernel_size=(filter_height, config.embedding_dim),
                )
                for filter_height in config.filter_sizes
            ]
        )

        self.fc = nn.Linear(
            in_features=len(config.filter_sizes) * config.num_filters,
            out_features=config.output_dim,
        )
        self.dropout = nn.Dropout(config.dropout)

    def forward(self, x: torch.Tensor):
        # x: (batch_size, aligned_length), contents are id
        x = self.embedding(x)
        # x: (batch_size, aligned_length, embedding_dim), contents are wordvecs
        x = x.unsqueeze(1)
        # x: (batch_size, in_channels, aligned_length, embedding_dim)
        x = [torch.relu(conv(x)).squeeze(3) for conv in self.convs]
        # x: [(batch_size, num_filters, new_length), ...], new_length = aligned_length - filter_height + 1
        x = [nn.functional.max_pool1d(i, i.size(2)).squeeze(2) for i in x]
        # x: [(batch_size, num_filters), ...]
        x = torch.cat(x, dim=1)
        # x: (batch_size, num_filters * len(filter_sizes))
        x = self.fc(self.dropout(x))
        return x


class RNN_LSTM(nn.Module):
    def __init__(self, config: Config):
        super(RNN_LSTM, self).__init__()

        self.embedding = nn.Embedding(config.vocab_size, config.embedding_dim)
        self.embedding.weight.data.copy_(config.pretrained_embed)
        self.embedding.weight.requires_grad = config.update_w2v

        self.num_layers = config.num_layers
        self.hidden_size = config.hidden_size
        self.lstm = nn.LSTM(
            config.embedding_dim,
            self.hidden_size,
            self.num_layers,
            dropout=config.dropout,
            batch_first=True,
            bidirectional=True,
        )

        internal = config.internal
        self.fc1 = nn.Linear(config.hidden_size * 2, internal)
        self.fc2 = nn.Linear(internal, config.output_dim)
        self.dropout = nn.Dropout(config.dropout)

    def forward(self, x: torch.Tensor):
        x = self.embedding(x)
        _, (x, _) = self.lstm(x)  # short-term memory (hidden)
        # x: (num_layers * 2, batch_size, hidden_size)
        f = x[-2, :, :]  # last layer forward
        b = x[-1, :, :]  # last layer backward
        x = torch.cat((f, b), dim=-1)
        # x: (batch_size, hidden_size * 2)
        x = torch.relu(self.fc1(x))
        x = self.fc2(self.dropout(x))
        return x


class RNN_GRU(nn.Module):
    def __init__(self, config: Config):
        super(RNN_GRU, self).__init__()

        self.embedding = nn.Embedding(config.vocab_size, config.embedding_dim)
        self.embedding.weight.data.copy_(config.pretrained_embed)
        self.embedding.weight.requires_grad = config.update_w2v

        self.num_layers = config.num_layers
        self.hidden_size = config.hidden_size
        self.gru = nn.GRU(
            input_size=config.embedding_dim,
            hidden_size=config.hidden_size,
            num_layers=config.num_layers,
            dropout=config.dropout,
            batch_first=True,
            bidirectional=True,
        )

        internal = config.internal
        self.fc1 = nn.Linear(2 * config.hidden_size, internal)
        self.fc2 = nn.Linear(internal, config.output_dim)
        self.dropout = nn.Dropout(config.dropout)

    def forward(self, x: torch.Tensor):
        x = self.embedding(x)
        # x: (batch_size, aligned_length, embedding_dim), contents are wordvecs
        _, x = self.gru(x)
        # x: (num_layers * 2, batch_size, hidden_size)
        f = x[-2, :, :]  # last layer forward
        b = x[-1, :, :]  # last layer backward
        x = torch.cat((f, b), dim=-1)
        # x: (batch_size, hidden_size * 2)
        x = torch.relu(self.fc1(x))
        x = self.fc2(self.dropout(x))
        return x


class MLP(nn.Module):
    def __init__(self, config: Config):
        super(MLP, self).__init__()

        self.embedding = nn.Embedding(config.vocab_size, config.embedding_dim)
        self.embedding.weight.data.copy_(config.pretrained_embed)
        self.embedding.weight.requires_grad = config.update_w2v

        self.dropout = nn.Dropout(config.dropout)
        self.fc1 = nn.Linear(config.embedding_dim, config.hidden_size)
        self.fc2 = nn.Linear(config.hidden_size, config.output_dim)

    def forward(self, x: torch.Tensor):
        x = self.embedding(x)
        # x: (batch_size, aligned_length, embedding_dim), contents are wordvecs
        x = self.fc1(x)
        x = torch.relu(x)
        # x: (batch_size, aligned_length, hidden_size)
        x = torch.max(x, dim=1).values
        # x: (batch_size, hidden_size)
        x = self.dropout(x)
        x = self.fc2(x)
        return x
