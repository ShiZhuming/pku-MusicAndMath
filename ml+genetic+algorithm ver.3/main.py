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
from sklearn.neural_network import MLPClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from midi_coding import *
from music_net import *
import warnings

class Individual():
	'''
	个体类
	'''
	def __init__(self, learning_machine, len_of_melody):
		# 参数为学习器，旋律片段长度
		self.learning_machine = learning_machine
		self.len_of_melody = len_of_melody
		self.pitch_code = np.zeros((len_of_melody,3))
		self.fitness = 0
		self.initialize()

	def initialize(self):
		# 初始化个体
		len_of_melody = self.len_of_melody
		duration_list = [120] * (int(len_of_melody*0.75)) + [240] * (int(len_of_melody*0.2)) +  [360] * (int(len_of_melody*0.05))
		self.pitch_code = np.array([[random.choice(duration_list), random.randint(60, 84), random.randint(80,127)] for i in range(self.len_of_melody)])

	def cal_fitness(self):
		# 计算适应度（适应度=利用学习器分类到类型1的概率）
		fitness_num = self.learning_machine.predict_proba(self.pitch_code.reshape(self.len_of_melody*3).copy())[0][1]
		self.fitness = fitness_num
		return fitness_num

	def indi_mutation(self):
		# 变异
		j = random.randint(0, self.len_of_melody-1)
		duration_list = [120] * (int(self.len_of_melody*0.75)) + [240] * (int(self.len_of_melody*0.2)) +  [360] * (int(self.len_of_melody*0.05))
		self.pitch_code[j] = np.array([random.choice(duration_list), random.randint(60, 84), random.randint(80,127)]).copy()
		self.cal_fitness()

	def indi_transposition(self):
		# 移调
		j = random.randint(0, 24)
		self.pitch_code[:,1] = 60 + ((self.pitch_code[:,1]+j-60)%24)
		self.cal_fitness()

	def indi_inversion(self):
		# 倒影
		j = random.randint(0, 24)
		self.pitch_code[:,1] = 60 + ((2*j-(self.pitch_code[:,1]-60))+24)%24
		self.cal_fitness()

	def indi_reversion(self):
		# 逆行
		self.pitch_code = self.pitch_code[::-1]
		self.cal_fitness()


	def copy(self):
		# 深拷贝
		tmp_individual = Individual(self.learning_machine, self.len_of_melody)
		tmp_individual.pitch_code = self.pitch_code.copy()
		return tmp_individual

class Population():
	'''
	种群类
	'''
	def __init__(self, num_populations, learning_machine, len_of_melody, mutation_probability=0.4):
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
		# 归一化
		self.fitness_array = np.zeros(self.num_populations)
		for i in range(self.num_populations):
			self.fitness_array[i] = self.populations[i].cal_fitness()
		self.fitness_array = self.fitness_array/(np.sum(self.fitness_array))

	def cumulative(self):
		# 为轮盘赌算法做准备
		self.normalization()
		cum_array = np.zeros(self.fitness_array.shape)
		total_num = 0
		for i in range(0, self.num_populations):
			total_num += self.fitness_array[i]
			cum_array[i] = total_num
		return cum_array

	def selection(self):
		# 选择
		cum_array = self.cumulative()
		new_populations = []
		self.num_populations = int(self.num_populations/2)
		for i in range(self.num_populations):
			survival_num = random.random()
			j = 0
			while (cum_array[j] < survival_num):
				j += 1
			new_populations.append(self.populations[j].copy())

		self.populations = new_populations
		self.normalization()

	def mutation(self):
		# 变异 
		len_of_melody = self.len_of_melody
		duration_list = [120] * (int(len_of_melody*0.75)) + [240] * (int(len_of_melody*0.2)) +  [360] * (int(len_of_melody*0.05))
		for i in range(self.num_populations):
			mutation_num = random.random()
			if mutation_num < self.mutation_probability/4:
				self.populations[i].indi_mutation()
			elif mutation_num < self.mutation_probability/2:
				self.populations[i].indi_transposition()
			elif mutation_num < self.mutation_probability*3/4:
				self.populations[i].indi_inversion()
			elif mutation_num < self.mutation_probability:
				self.populations[i].indi_reversion()
			else:
				continue
		self.normalization()

	def cross_over(self):
		# 交叉
		for i in range(int(self.num_populations/2)):
			random_index = random.randint(0, self.len_of_melody-1)
			tmp_individual_1 = Individual(self.learning_machine, self.len_of_melody)
			tmp_individual_2 = Individual(self.learning_machine, self.len_of_melody)
			tmp_individual_1.pitch_code[:random_index] = self.populations[2*i].pitch_code[:random_index]
			tmp_individual_1.pitch_code[random_index:] = self.populations[2*i+1].pitch_code[random_index:]
			tmp_individual_2.pitch_code[:random_index] = self.populations[2*i+1].pitch_code[:random_index]
			tmp_individual_2.pitch_code[random_index:] = self.populations[2*i].pitch_code[random_index:]
			tmp_individual_1.cal_fitness()
			tmp_individual_2.cal_fitness()
			self.populations.append(tmp_individual_1)
			self.populations.append(tmp_individual_2)
		self.num_populations = self.num_populations*2
		self.normalization()

	def get_mean_fitness(self):
		# 计算平均适应度
		sum_num = 0
		for i in range(self.num_populations):
			sum_num += self.populations[i].cal_fitness()
		return sum_num/self.num_populations

	def iterate(self):
		self.mutation()
		self.cross_over()
		self.selection()

	def get_best_individual_code(self):
		# 选取最好的个体
		sorted_populations = sorted(self.populations, key = lambda individual:individual.fitness, reverse = True)
		i = 0
		while (sorted_populations[i].pitch_code[0][1] == 0 and i < self.num_populations):
			i += 1
		if i == self.num_populations:
			best_list = self.populations[0].pitch_code.tolist()
			best_list[0][1] = 60
		else:
			best_list = self.populations[i].pitch_code.tolist()
		return [[int(i[0]),int(i[1]), int(i[2])] for i in best_list]

if __name__ == '__main__':
	warnings.filterwarnings("ignore")# 隐藏warning信息
	
	# 将训练好的模型导入
	logreg = joblib.load('models/logreg.model')
	mlp = joblib.load('models/mlp.model')
	forest = joblib.load('models/forest.model')


	# 使用 CNN
	print("\n----Using CNN net----\n")
	net = MusicNet()
	net.load_state_dict(torch.load('models/music_net.tar'))
	test_population = Population(30, net, 64)
	list_mean = []
	for i in range(1000):
		test_population.iterate()
		list_mean.append(test_population.get_mean_fitness())
	plt.figure()
	plt.plot(list_mean)
	print("Best individual: {}".format(test_population.get_best_individual_code()))
	reconversion(test_population.get_best_individual_code(), "output_melody/output_cnn.midi" )
	plt.savefig("output_figure/output_cnn.png")

	# 使用 logreg
	print("\n----Using logreg----\n")
	test_population = Population(30, logreg, 64)
	list_mean = []
	for i in range(1000):
		test_population.iterate()
		list_mean.append(test_population.get_mean_fitness())
	plt.figure()
	plt.plot(list_mean)
	print("Best individual: {}".format(test_population.get_best_individual_code()))
	reconversion(test_population.get_best_individual_code(), "output_melody/output_logreg.midi" )
	plt.savefig("output_figure/output_logreg.png")

	# 使用mlp
	print("\n----Using mlp----\n")
	test_population = Population(30, mlp, 64)
	list_mean = []
	for i in range(1000):
		test_population.iterate()
		list_mean.append(test_population.get_mean_fitness())
	plt.figure()
	plt.plot(list_mean)
	print("Best individual: {}".format(test_population.get_best_individual_code()))
	reconversion(test_population.get_best_individual_code(), "output_melody/output_mlp.midi" )
	plt.savefig("output_figure/output_mlp.png")

	# 使用Random Forest
	print("\n----Using forest----\n")
	test_population = Population(30, forest, 64)
	list_mean = []
	for i in range(1000):
		test_population.iterate()
		list_mean.append(test_population.get_mean_fitness())
	plt.figure()
	plt.plot(list_mean)
	print("Best individual: {}".format(test_population.get_best_individual_code()))
	reconversion(test_population.get_best_individual_code(), "output_melody/output_forest.midi" )
	plt.savefig("output_figure/output_forest.png")
