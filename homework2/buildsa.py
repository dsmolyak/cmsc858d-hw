import argparse
import numpy as np
import time

from pysuffixarray.core import SuffixArray
from pysam import FastaFile


def buildsa(reference_seq: str):
    sa = SuffixArray(reference_seq)
    result = np.array(sa.suffix_array())
    return result


if __name__ == '__main__':
    # Read args
    parser = argparse.ArgumentParser(description='Build Suffix Array')
    parser.add_argument('reference', metavar='reference', type=str)
    parser.add_argument('output', metavar='output', type=str)
    parser.add_argument('--preftab', type=int)

    args = parser.parse_args()
    # print('ref', args.reference)
    # print('out', args.output)
    # if args.preftab:
    #     print('preftab', args.preftab)

    # read FASTA file
    sequences_object = FastaFile(args.reference)
    tmp_sequence = sequences_object.fetch(sequences_object.references[0])
    tmp_sequence_short = tmp_sequence[:int(len(tmp_sequence)/12)]
    curr_sequence = tmp_sequence_short + '&'  # 'abracadabral' + '&'  #

    start_time = time.time()
    print('start')
    # Build suffix array and save
    sa = buildsa(curr_sequence)

    # Build preftab
    if args.preftab:
        k = args.preftab
        preftab_keys = []
        preftab_intervals = []
        curr_pref = None
        curr_interval = np.zeros(2)
        for i in range(1, len(sa)):
            if sa[i]+k >= len(curr_sequence):
                if curr_pref is not None:
                    curr_interval[1] = i
                    preftab_keys.append(curr_pref)
                    preftab_intervals.append(curr_interval.copy())
                    curr_pref = None
                    curr_interval = np.zeros(2)
                continue
            pref = curr_sequence[sa[i]:sa[i]+k]
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

        preftab_keys, preftab_intervals = np.array(preftab_keys), np.array(preftab_intervals)

        # print(sa)
        # print(preftab_keys)
        # print(preftab_intervals)

        total_time = time.time() - start_time
        print('total_time', total_time)

        # Save suffix array
        np.save(f'out/{args.output}_sa.npy', sa)
        np.save(f'out/{args.output}_preftab_keys.npy', preftab_keys)
        np.save(f'out/{args.output}_preftab_intervals.npy', preftab_intervals)




