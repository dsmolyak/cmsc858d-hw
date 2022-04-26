import argparse
import numpy as np
import pickle
from os.path import exists
import time

from pysam import FastaFile


def run_query_naive(query_seq, sa, sequence, preftab, k):
    query_len = len(query_seq)
    start_left = 0
    start_right = len(sa)
    if preftab:
        interval = preftab[query_seq[:k]]
        start_left, start_right = interval[0], interval[1]

    found = False
    # Try finding left side of range
    result_left = 0
    left, right = start_left, start_right
    while left < right:
        center = int((left + right) / 2)
        curr_str = sequence[sa[center]:sa[center]+query_len]
        prev_str = sequence[sa[center-1]:sa[center-1]+query_len]
        if curr_str == query_seq and prev_str != query_seq:
            found = True
            result_left = center
            break
        elif curr_str < query_seq:
            left = center
        else:
            right = center
    if not found:
        return []

    # Find right side of range
    result_right = 0
    left, right = start_left, start_right
    while left < right:
        center = int((left + right) / 2)
        curr_str = sequence[sa[center]:sa[center] + query_len]
        next_str = sequence[sa[center + 1]:sa[center + 1] + query_len]
        if curr_str == query_seq and next_str != query_seq:
            result_right = center
            break
        elif curr_str <= query_seq:
            left = center
        else:
            right = center

    return [sa[i] for i in range(result_left, result_right+1)]


def find_lcp(str1, str2):
    min_len = min(len(str1), len(str2))
    for i in range(0, min_len):
        if str1[i] != str2[i]:
            return i
    return min_len


def run_query_simpaccel(query_seq, sa, sequence, preftab, k):
    query_len = len(query_seq)
    start_left, start_right = 0, len(sa)
    start_lcp = 0
    if preftab:
        interval = preftab[query_seq[:k]]
        start_left, start_right = interval[0], interval[1]
        start_lcp = k

    found = False
    # Try finding left side of range
    result_left = 0
    left, right = start_left, start_right
    left_lcp, right_lcp = start_lcp, start_lcp
    while left < right:
        center = int((left + right) / 2)
        center_lcp = min(left_lcp, right_lcp)
        # print(left, center, right, left_lcp, right_lcp)
        curr_str = sequence[sa[center]+center_lcp:sa[center]+query_len]
        prev_str = sequence[sa[center-1]+center_lcp:sa[center-1]+query_len]
        curr_lcp = find_lcp(query_seq[center_lcp:], curr_str) + center_lcp
        prev_lcp = find_lcp(query_seq[center_lcp:], prev_str) + center_lcp
        if curr_lcp == query_len and prev_lcp != query_len:
            found = True
            result_left = center
            break
        elif sa[center] + curr_lcp >= len(sequence) or (curr_lcp < query_len and
                                                        sequence[sa[center]+curr_lcp] < query_seq[curr_lcp]):
            left = center
            left_lcp = curr_lcp
        else:
            right = center
            right_lcp = curr_lcp
    if not found:
        return []

    # Find right side of range
    result_right = 0
    left, right = start_left, start_right
    left_lcp, right_lcp = start_lcp, start_lcp
    while left < right:
        center = int((left + right) / 2)
        center_lcp = min(left_lcp, right_lcp)
        curr_str = sequence[sa[center] + center_lcp:sa[center] + query_len]
        next_str = sequence[sa[center + 1] + center_lcp:sa[center + 1] + query_len]
        curr_lcp = find_lcp(query_seq[center_lcp:], curr_str) + center_lcp
        next_lcp = find_lcp(query_seq[center_lcp:], next_str) + center_lcp
        if curr_lcp == query_len and next_lcp != query_len:
            result_right = center
            break
        elif sa[center] + curr_lcp >= len(sequence) or curr_lcp >= query_len or \
                sequence[sa[center]+curr_lcp] <= query_seq[curr_lcp]:
            left = center
            left_lcp = curr_lcp
        else:
            right = center
            right_lcp = curr_lcp

    return [sa[i] for i in range(result_left, result_right+1)]


if __name__ == '__main__':
    # Read args
    parser = argparse.ArgumentParser(description='Query Suffix Array')
    parser.add_argument('index', type=str)
    parser.add_argument('queries', type=str)
    parser.add_argument('query_mode', type=str)
    parser.add_argument('output', type=str)

    args = parser.parse_args()

    # Loading suffix array files
    sequence = pickle.load(open(f'out_sa/{args.index}_sequence.pickle', 'rb'))
    sa = np.load(f'out_sa/{args.index}_sa.npy')
    use_preftab = exists(f'out_sa/{args.index}_preftab_keys.npy')
    preftab = {}
    k = 0
    if use_preftab:
        preftab_keys = np.load(f'out_sa/{args.index}_preftab_keys.npy')
        k = len(preftab_keys[0])
        preftab_intervals = np.load(f'out_sa/{args.index}_preftab_intervals.npy')
        for i in range(0, len(preftab_keys)):
            preftab[preftab_keys[i]] = preftab_intervals[i]

    # print(sequence)
    # print(sa)
    # print(run_query_simpaccel('abra', sa, sequence, preftab, k))

    # Loading queries
    queries_fasta = FastaFile(args.queries)

    # Running queries
    results = {}
    for i in range(0, len(queries_fasta.references)):
        if i >= 10:
            break
        query_name = queries_fasta.references[i]
        query_seq = queries_fasta.fetch(query_name)
        if args.query_mode == 'naive':
            results[query_name] = run_query_naive(query_seq, sa, sequence, preftab, k)
        elif args.query_mode == 'simpaccel':
            results[query_name] = run_query_simpaccel(query_seq, sa, sequence, preftab, k)
        else:
            print('not a valid query method')

    with open(f'./out_query/{args.output}.txt', 'w') as f:
        for query_name, result_list in results.items():
            result_str = f'{query_name}\t{len(result_list)}'
            for result in result_list:
                result_str += f'\t{result}'
            f.write(result_str+'\n')