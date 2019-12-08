#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author:Lin
# For training models

import numpy as np
import random
from sklearn.linear_model import LogisticRegression
from sklearn.neural_network import MLPClassifier
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.externals import joblib
from midi_coding import *

def buiding_train_set(num_of_train, len_of_melody, track_list):
	'''
	从所有音轨中随机选取固定长度的片段，作为正训练集
	'''
	current_index = 0
	train_set_list = []
	i = 0
	while (len(train_set_list) < num_of_train):
		if (len(track_list[current_index]) > len_of_melody+1):
			random_index = random.randint(0, len(track_list[current_index])-len_of_melody-1)
			list_i = track_list[current_index][random_index:random_index+len_of_melody]
			if (list_i[0] != 0):
				train_set_list.append(list_i)
		current_index = (current_index+1)%(len(track_list))
		i += 1
	train_set_array = np.array(train_set_list)
	#print("train_set_array.shape: {}".format(train_set_array.shape))
	return train_set_array

def buiding_bad_train_set(num_of_train, len_of_melody, track_list):
	'''
	随机生成固定长度的片段，作为负训练集
	'''
	duration_list = [120] * (int(len_of_melody*0.75)) + [240] * (int(len_of_melody*0.2)) +  [360] * (int(len_of_melody*0.05))
	train_bad_list = []
	for i in range(num_of_train):
		train_bad_i = [[random.choice(duration_list), random.randint(60, 84), random.randint(80,127)] for i in range(len_of_melody)]
		train_bad_list.append(train_bad_i)
	train_bad_array = np.array(train_bad_list)
	#print("train_bad_set_array.shape: {}".format(train_bad_array.shape))
	return train_bad_array

def buiding_total_train_set(num_of_train, len_of_melody, track_list):
	'''
	整理训练集
	'''
	train_set_array = buiding_train_set(num_of_train, len_of_melody, track_list)
	train_bad_array = buiding_bad_train_set(num_of_train, len_of_melody, track_list)
	total_train_set = np.concatenate((train_set_array, train_bad_array), axis=0)
	#print("total_train_set.shape: {}".format(total_train_set.shape))

	target_train_set = np.array([[1] for i in range(num_of_train)])
	target_train_bad = np.array([[0] for i in range(num_of_train)])
	total_target_train = np.concatenate((target_train_set, target_train_bad), axis=0)
	#print("total_target_train.shape: {}".format(total_target_train.shape))

	return total_train_set, total_target_train

def flattern_train_set(num_of_train, len_of_melody, train_set_array):
	'''
	将训练集中的数据打平，以便后续训练
	'''
	train_new_set_array = train_set_array.copy().reshape(num_of_train, len_of_melody*3)
	return train_new_set_array


if __name__ == '__main__':
	track_list = np.load("train_set/jsbachcoding_bach.npy").tolist()
	train_set, target_set = buiding_total_train_set(1000, 64, track_list)
	new_train_set = flattern_train_set(2000, 64, train_set)
	X_train, X_test, y_train, y_test = train_test_split(\
		new_train_set, target_set, stratify=target_set, random_state=42)
	print("X_train.shape: {}, X_test.shape: {}\n, y_train.shape: {}, y_test.shape: {}".format(
		X_train.shape, X_test.shape, y_train.shape, y_test.shape))

	# 使用LogisticRegeression进行训练
	print("\n----Using LogisticRegression----\n")
	logreg = LogisticRegression(C=100).fit(X_train, y_train)
	print("Training set score: {:.3f}".format(logreg.score(X_train, y_train)))
	print("Test set score: {:.3f}".format(logreg.score(X_test, y_test)))
	joblib.dump(logreg,'models/logreg.model')#将模型数据直接存储，方便后续直接使用

	# 使用MLP进行训练
	print("\n----Using MLPClassifier----\n")
	mlp = MLPClassifier(solver='adam', activation='tanh',\
			random_state=0, hidden_layer_sizes=[1000, 100])
	mlp.fit(X_train, y_train)
	print("Accuracy on training set: {:.3f}".format(mlp.score(X_train, y_train)))
	print("Accuracy on test set: {:.3f}".format(mlp.score(X_test, y_test)))
	joblib.dump(mlp,'models/mlp.model')

	# 使用随机森林进行训练
	print("\n----Using random forest----\n")
	forest = RandomForestClassifier(n_estimators=100, random_state=0)
	forest.fit(X_train, y_train)
	print("Training set score: {}".format(forest.score(X_train, y_train)))
	print("Test set score: {}".format(forest.score(X_test, y_test)))
	joblib.dump(forest,'models/forest.model')

