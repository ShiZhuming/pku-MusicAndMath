#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author:Lin
# For composing

import random
import numpy as np
import matplotlib.pyplot as plt
import midi
import mido
import pretty_midi
import numpy as np
import random
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.neural_network import MLPClassifier
from train_set_construction import *
from midi_coding import *
import warnings

class Individual():
	'''
	class for individual
	'''
	def __init__(self, learning_machine, len_of_melody):
		self.learning_machine = learning_machine
		self.len_of_melody = len_of_melody
		self.pitch_code = np.zeros(len_of_melody)
		self.fitness = 0
		self.initialize()

	def initialize(self):
		range_list = [0, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,\
			81, 82, 83, 84]
		for i in range(self.len_of_melody):
			self.pitch_code[i] = range_list[random.randint(0, 25)]

	def cal_fitness(self):
		fitness_num = self.learning_machine.predict_proba(self.pitch_code)[0][1]
		self.fitness = fitness_num
		return fitness_num

	def copy(self):
		tmp_individual = Individual(self.learning_machine, self.len_of_melody)
		tmp_individual.pitch_code = np.array(self.pitch_code.tolist()[:])
		return tmp_individual

class Population():
	'''
	class for population
	'''
	def __init__(self, num_populations, learning_machine, len_of_melody, mutation_probability=0.1):
		self.num_populations = num_populations
		self.populations = []
		self.len_of_melody = len_of_melody
		self.learning_machine = learning_machine
		self.mutation_probability = mutation_probability
		self.fitness_array = np.zeros(num_populations)
		for i in range(num_populations):
			self.populations.append(Individual(learning_machine, len_of_melody))
			self.fitness_array[i] = self.populations[i].cal_fitness()
		self.normalization()

	def normalization(self):
		for i in range(self.num_populations):
			self.fitness_array[i] = self.populations[i].cal_fitness()
		self.fitness_array = self.fitness_array/(np.sum(self.fitness_array))

	def cumulative(self):
		self.normalization()
		cum_array = np.zeros(self.fitness_array.shape)
		total_num = 0
		for i in range(0, self.num_populations):
			total_num += self.fitness_array[i]
			cum_array[i] = total_num
		return cum_array

	def selection(self):
		cum_array = self.cumulative()
		new_populations = []
		for i in range(self.num_populations):
			survival_num = random.random()
			j = 0
			while (cum_array[j] < survival_num):
				j += 1
			new_populations.append(self.populations[j].copy())

		self.populations = new_populations
		self.normalization()

	def another_mutation(self):
		'''
		another kind of mutation
		'''
		range_list = [0, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,\
			81, 82, 83, 84]
		new_population = []
		for i in range(self.num_populations):
			for j in range(self.len_of_melody):
				mutation_num = random.random()
				if mutation_num < self.mutation_probability:
					tmp_individual = self.populations[i]
					tmp_individual.pitch_code[j] = range_list[random.randint(0, 25)]
					tmp_individual.fitness = tmp_individual.cal_fitness()
				else:
					continue
		self.normalization()

	def mutation(self):
		range_list = [0, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,\
			81, 82, 83, 84]
		for i in range(self.num_populations):
			mutation_num = random.random()
			if mutation_num < self.mutation_probability:
				j = random.randint(0, self.len_of_melody-1)
				self.populations[i].pitch_code[j] = range_list[random.randint(0, 25)]
				self.populations[i].cal_fitness()
		self.normalization()

	def get_mean_fitness(self):
		sum_num = 0
		for i in range(self.num_populations):
			sum_num += self.populations[i].cal_fitness()
		return sum_num/self.num_populations

	def get_best_individual_code(self):
		sorted_populations = sorted(self.populations, key = lambda individual:individual.fitness, reverse = True)
		i = 0
		while (sorted_populations[i].pitch_code[0] == 0 and i < self.num_populations):
			i += 1
		if i == self.num_populations:
			best_list = self.populations[0].pitch_code.tolist()
			best_list[0] = 60
		else:
			best_list = self.populations[i].pitch_code.tolist()
		return [int(i) for i in best_list]

if __name__ == '__main__':
	warnings.filterwarnings("ignore")# 隐藏warning信息
	track_list = np.load("训练集/jsbachcoding_bach.npy").tolist()
	train_set, target_set = buiding_total_train_set(1000, 32, track_list)
	X_train, X_test, y_train, y_test = train_test_split(\
		train_set, target_set, stratify=target_set, random_state=42)

	# using logreg
	print("\n----Using Logreg----\n")
	logreg = LogisticRegression(C=100).fit(X_train, y_train)
	print("Training set score: {:.3f}".format(logreg.score(X_train, y_train)))
	print("Test set score: {:.3f}".format(logreg.score(X_test, y_test)))
	print("coeff: {}".format(logreg.coef_))
	print("intercept: {}".format(logreg.intercept_))
	print("predict_proba: {}".format(logreg.predict_proba(X_test[0])))
	print("predict: {}".format(logreg.predict(X_test[0])))

	test_population = Population(50, logreg, 32)
	list_mean = []
	for i in range(100):
		test_population.mutation()
		test_population.selection()
		list_mean.append(test_population.get_mean_fitness())
	#plt.plot(list_mean)
	print("Best individual: {}".format(test_population.get_best_individual_code()))
	reconversion(test_population.get_best_individual_code(), "output_logreg.midi" )
	#plt.show()

	# using mlp
	print("\n----Using mlp----\n")
	mlp = MLPClassifier(solver='adam', activation='tanh',\
			random_state=0, hidden_layer_sizes=[1000, 100])
	mlp.fit(X_train, y_train)
	print("Accuracy on training set: {:.2f}".format(mlp.score(X_train, y_train)))
	print("Accuracy on test set: {:.2f}".format(mlp.score(X_test, y_test)))
	print("predict_proba: {}".format(mlp.predict_proba(X_test[0])))
	print("predict: {}".format(mlp.predict(X_test[0])))

	test_population_2 = Population(50, logreg, 32)
	list_mean = []
	for i in range(500):
		test_population_2.mutation()
		test_population_2.selection()
		list_mean.append(test_population_2.get_mean_fitness())
	plt.plot(list_mean)
	print("Best individual: {}".format(test_population_2.get_best_individual_code()))
	reconversion(test_population_2.get_best_individual_code(), "output_mlp.midi" )
	plt.show()