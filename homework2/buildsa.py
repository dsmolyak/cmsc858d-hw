import argparse
import numpy as np
import time
import pickle
from os.path import getsize

from pydivsufsort import divsufsort
from pysam import FastaFile


def build_preftab(sa, k, curr_sequence):
    preftab_keys = []
    preftab_intervals = []
    curr_pref = None
    curr_interval = np.zeros(2)
    for i in range(1, len(sa)):
        if sa[i] + k >= len(curr_sequence):
            if curr_pref is not None:
                curr_interval[1] = i
                preftab_keys.append(curr_pref)
                preftab_intervals.append(curr_interval.copy())
                curr_pref = None
                curr_interval = np.zeros(2)
            continue
        pref = curr_sequence[sa[i]:sa[i] + k]
        if curr_pref is None:
            curr_interval[0] = i
            curr_pref = pref
        if pref != curr_pref:
            curr_interval[1] = i
            preftab_keys.append(curr_pref)
            preftab_intervals.append(curr_interval.copy())

            curr_pref = pref
            curr_interval = np.zeros(2)
            curr_interval[0] = i
    if curr_pref is not None:
        preftab_keys.append(curr_pref)
        curr_interval[1] = len(sa)
        preftab_intervals.append(curr_interval)

    return np.array(preftab_keys), np.array(preftab_intervals)


# ecoli.fa ecoli --preftab 15
if __name__ == '__main__':
    # Read args
    parser = argparse.ArgumentParser(description='Build Suffix Array')
    parser.add_argument('reference', metavar='reference', type=str)
    parser.add_argument('output', metavar='output', type=str)
    parser.add_argument('--preftab', type=int)

    args = parser.parse_args()

    # read FASTA file
    sequences_object = FastaFile('data/' + args.reference)
    tmp_sequence = sequences_object.fetch(sequences_object.references[0])
    tmp_sequence_short = tmp_sequence[:int(len(tmp_sequence)/12)]
    curr_sequence = tmp_sequence + '$'  # 'abracadabracadabra'

    # Generating Report Data
    # f_sa = open('sa_time.csv', 'w')
    # f_preftab = open('preftab_time.csv', 'w')
    # f_sa.write('seq_len,sa_time,sa_size\n')
    # f_preftab.write('seq_len,k,preftab_time,preftab_size\n')
    # for seq_len in [1e5, 2.5e5, 5e5, 1e6, 2e6]:
    #     seq_len = int(seq_len)
    #     curr_sequence = tmp_sequence[:seq_len]
    #     start_time = time.time()
    #     sa = np.array(divsufsort(curr_sequence))
    #     sa_time = time.time() - start_time
    #     np.save(f'out_sa/{args.output}_{seq_len}_sa.npy', sa)
    #     pickle.dump(curr_sequence, open(f'out_sa/{args.output}_{seq_len}_sequence.pickle', 'wb'))
    #     sa_size = getsize(f'out_sa/{args.output}_{seq_len}_sa.npy')
    #     row = f'{seq_len},{sa_time},{sa_size}\n'
    #     f_sa.write(row)
    #     for k in [5, 10, 20, 30]:
    #         start_time = time.time()
    #         preftab_keys, preftab_intervals = build_preftab(sa, k, curr_sequence)
    #         preftab_time = time.time() - start_time
    #
    #         np.save(f'out_sa/{args.output}_{seq_len}_{k}_preftab_keys.npy', preftab_keys)
    #         np.save(f'out_sa/{args.output}_{seq_len}_{k}_preftab_intervals.npy', preftab_intervals)
    #         preftab_keys_size = getsize(f'out_sa/{args.output}_{seq_len}_{k}_preftab_keys.npy')
    #         preftab_intervals_size = getsize(f'out_sa/{args.output}_{seq_len}_{k}_preftab_intervals.npy')
    #         preftab_size = preftab_keys_size + preftab_intervals_size
    #
    #         row = f'{seq_len},{k},{preftab_time},{preftab_size}\n'
    #         print(row)
    #         f_preftab.write(row)

    start_time = time.time()
    print('start')

    sa = np.array(divsufsort(curr_sequence))

    # Build preftab
    if args.preftab:
        preftab_keys, preftab_intervals = build_preftab(sa, args.preftab, curr_sequence)

    total_time = time.time() - start_time
    print('total_time', total_time)

    # Save suffix array
    pickle.dump(curr_sequence, open(f'out_sa/{args.output}_sequence.pickle', 'wb'))
    np.save(f'out_sa/{args.output}_sa.npy', sa)
    if args.preftab:
        np.save(f'out_sa/{args.output}_preftab_keys.npy', preftab_keys)
        np.save(f'out_sa/{args.output}_preftab_intervals.npy', preftab_intervals)




