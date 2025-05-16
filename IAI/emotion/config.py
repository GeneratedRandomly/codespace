import argparse
import torch
from models import Config, RNN_LSTM, CNN, MLP, RNN_GRU

config = Config()
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")


def parser_data():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-lr",
        dest="learning_rate",
        type=float,
        default=1e-3,
        help="The starting learning rate, which will decay exponentially",
    )
    parser.add_argument(
        "-e",
        dest="epochs",
        type=int,
        default=10,
        help="epochs of training, default is 10",
    )
    parser.add_argument(
        "-a",
        dest="alignLen",
        type=int,
        default=80,
        help="align sentence to length, default is 80",
    )
    parser.add_argument(
        "-b",
        dest="batch_size",
        type=int,
        default=50,
        help="batch size",
    )
    parser.add_argument(
        "-n",
        dest="neural_name",
        type=str,
        default="CNN",
        help="neural name, default is CNN, you can choose RNN",
    )
    parser.add_argument(
        "-hs",
        dest="hidden_size",
        type=int,
        default=128,
        help="hidden size of RNN, default is 128",
    )
    parser.add_argument(
        "-l",
        dest="num_layers",
        type=int,
        default=2,
        help="number of layers of RNN, default is 2",
    )
    parser.add_argument(
        "-d",
        dest="dropout",
        type=float,
        default=0.3,
        help="dropout rate of RNN, default is 0.3",
    )
    parser.add_argument(
        "-f",
        dest="num_filters",
        type=int,
        default=64,
        help="number of filters of CNN, default is 64",
    )
    parser.add_argument(
        "-u",
        dest="update_w2v",
        action="store_true",
        help="whether to update word2vec during training, default is False",
    )
    parser.add_argument(
        "-i",
        dest="internal",
        type=int,
        default=32,
        help="internal size of MLP, default is 32",
    )
    args = parser.parse_args()
    learning_rate = args.learning_rate
    epochs = args.epochs
    alignLen = args.alignLen
    batch_size = args.batch_size
    neural_name = args.neural_name
    config.hidden_size = args.hidden_size
    config.num_layers = args.num_layers
    config.dropout = args.dropout
    config.num_filters = args.num_filters
    config.update_w2v = args.update_w2v
    config.internal = args.internal
    if neural_name == "RNN_LSTM":
        model = RNN_LSTM(config).to(device)
    elif neural_name == "RNN_GRU":
        model = RNN_GRU(config).to(device)
    elif neural_name == "CNN":
        model = CNN(config).to(device)
    elif neural_name == "MLP":
        model = MLP(config).to(device)
    else:
        print("please choose neural network type")
        exit(1)
    return learning_rate, epochs, alignLen, batch_size, neural_name, model, config
