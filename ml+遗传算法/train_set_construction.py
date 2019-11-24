#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author:Lin
# For constructing a training set

import numpy as np
import random
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.neural_network import MLPClassifier

def buiding_train_set(num_of_train, len_of_melody, track_list):
	'''
	buiding train_set with target 1
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
	buiding train_set with target 0
	'''
	range_list = [0, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,\
					81, 82, 83, 84]
	train_bad_list = []
	for i in range(num_of_train):
		train_bad_i = [range_list[random.randint(0,25)] for i in range(len_of_melody)]
		train_bad_list.append(train_bad_i)
	train_bad_array = np.array(train_bad_list)
	#print("train_bad_set_array.shape: {}".format(train_bad_array.shape))
	return train_bad_array

def buiding_total_train_set(num_of_train, len_of_melody, track_list):
	'''
	buiding total train set with target
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

if __name__ == '__main__':
	track_list = np.load("训练集/jsbachcoding_bach.npy").tolist()
	train_set, target_set = buiding_total_train_set(1000, 32, track_list)
	X_train, X_test, y_train, y_test = train_test_split(\
		train_set, target_set, stratify=target_set, random_state=42)

	# testing logreg
	logreg = LogisticRegression(C=100).fit(X_train, y_train)
	print("Training set score: {:.3f}".format(logreg.score(X_train, y_train)))
	print("Test set score: {:.3f}".format(logreg.score(X_test, y_test)))
	print("coeff: {}".format(logreg.coef_))
	print("intercept: {}".format(logreg.intercept_))
	print("predict_proba: {}".format(logreg.predict_proba(X_test[0])))
	print("predict: {}".format(logreg.predict(X_test[0])))

	# testing mlp
	mlp = MLPClassifier(solver='adam', activation='tanh',\
			random_state=0, hidden_layer_sizes=[1000, 100])
	mlp.fit(X_train, y_train)
	print("Accuracy on training set: {:.2f}".format(mlp.score(X_train, y_train)))
	print("Accuracy on test set: {:.2f}".format(mlp.score(X_test, y_test)))
	print("predict_proba: {}".format(mlp.predict_proba(X_test[0])))
	print("predict: {}".format(mlp.predict(X_test[0])))
