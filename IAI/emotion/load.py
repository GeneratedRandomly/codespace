import gensim
import numpy as np

word2id = {}


def getWord2Id():
    path = ["./Dataset/train.txt", "./Dataset/validation.txt", "./Dataset/test.txt"]
    i = 0
    for each in path:
        with open(each, encoding="utf-8", errors="ignore") as f:
            for line in f.readlines():
                sentence = line.strip().split()
                for word in sentence[1:]:
                    if word not in word2id:
                        word2id[word] = i
                        i += 1
    return word2id


def getId2Vec():
    if word2id == {}:
        getWord2Id()
    path = "./Dataset/wiki_word2vec_50.bin"
    # {"<word>": [0.0, ..., 0.49]}
    word2vec = gensim.models.KeyedVectors.load_word2vec_format(path, binary=True)
    vecsize = 50
    id2vecs = np.array(np.zeros([len(word2id) + 1, vecsize]))
    for word in word2id:
        try:
            id2vecs[word2id[word]] = word2vec[word]
        except Exception:
            pass
    return id2vecs


def getCorpus(path, alignLen=50):
    """
    alignLen: align the sentence to this length, trucate or pad\\
    return: contents, labels\\
    contents: [[word1id, word2id, ...], [word1id, word2id, ...], ...]\\
    labels: [label1, label2, ...]
    """
    contents = []
    labels = []
    with open(path, encoding="utf-8", errors="ignore") as f:
        for line in f.readlines():
            sentence = line.strip().split()
            label = int(sentence[0])
            words = sentence[1:]
            content = [word2id.get(w, 0) for w in words[:alignLen]]
            padding = alignLen - len(content)
            if padding > 0:
                content.extend([0] * padding)  # pad with 0
            labels.append(label)
            contents.append(content)
    contents = np.array(contents, dtype=np.int32)
    labels = np.array(labels, dtype=np.int32)
    return contents, labels
