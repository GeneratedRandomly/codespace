import os

if __name__ == "__main__":
    # os.system("python main.py -lr 2e-3 -a 80 -b 20 -hs 64 -d 0.2 -f 128")
    # os.system("python main.py -lr 2e-3 -e 1 -a 80 -b 20 -hs 64 -d 0.2 -f 128 -u")
    # os.system("python main.py -lr 2e-3 -a 80 -b 20 -hs 64 -f 128 -u -n MLP")
    os.system("python main.py -lr 2e-3 -a 80 -b 20 -hs 64 -d 0.2 -u -n RNN_GRU -i 64")
    os.system("python main.py -lr 2e-3 -a 80 -b 20 -hs 64 -d 0.2 -u -n RNN_LSTM -i 64")

    # rates = [5e-4, 1e-3, 2e-3, 5e-3]
    # for rate in rates:
    #     os.system(f"python main.py -lr {rate}")

    # epochs = [3, 5, 10, 15, 20]
    # for epoch in epochs:
    #     os.system(f"python main.py -e {epoch}")

    # aligns = [20, 50, 80, 120, 150]
    # for align in aligns:
    #     os.system(f"python main.py -a {align}")

    # sizes = [20, 35, 50, 65, 80]
    # for size in sizes:
    #     os.system(f"python main.py -b {size}")

    # names = ["CNN", "RNN_LSTM", "RNN_GRU", "MLP"]
    # for name in names:
    #     os.system(f"python main.py -n {name}")

    # hidden_sizes = [32, 64, 96, 128, 160]
    # for hidden_size in hidden_sizes:
    #     os.system(f"python main.py -n RNN_LSTM -hs {hidden_size}")

    # num_layers = [1, 2, 3]
    # for num_layer in num_layers:
    #     os.system(f"python main.py -n RNN_LSTM -l {num_layer}")

    # dropouts = [0.1, 0.2, 0.3, 0.4, 0.5]
    # for dropout in dropouts:
    #     os.system(f"python main.py -d {dropout}")

    # num_filters = [16, 32, 64, 96, 128]
    # for num_filter in num_filters:
    #     os.system(f"python main.py -f {num_filter}")

    # os.system("python main.py -u")
