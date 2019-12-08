import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable
import torchvision
import torchvision.transforms as transforms
import torch.optim as optim
from model_training import *

class MusicNet(nn.Module):
	'''
	一个适用于音乐学习的1d CNN 网络
	'''
	def __init__(self):
		super().__init__()
		# 卷积层1，输入通道数为3，输出通道数为6，卷积核为5
		self.conv1 = nn.Conv1d(3, 6, 5)
		# 池化层1
		self.pool1 = nn.MaxPool1d(2,2)
		# 卷积层2
		self.conv2 = nn.Conv1d(6, 16, 5)
		# 池化层2
		self.pool2 = nn.MaxPool1d(2,2)
		# 卷积层3
		self.conv3 = nn.Conv1d(16, 120, 5)
		# 池化层3
		self.pool3 = nn.MaxPool1d(2,2)
		# 全连接层1~3
		self.fc1 = nn.Linear(120*4, 120)
		self.fc2 = nn.Linear(120, 84)
		self.fc3 = nn.Linear(84, 2)
	
	def forward(self, x):
		x = self.pool1(F.relu(self.conv1(x)))
		x = self.pool2(F.relu(self.conv2(x)))
		x = self.pool3(F.relu(self.conv3(x)))
		x = x.view(-1, self.num_flat_features(x))
		x = F.relu(self.fc1(x))
		x = F.relu(self.fc2(x))
		x = self.fc3(x)
		return x
	
	def num_flat_features(self, x):
		size = x.size()[1:]
		num_features = 1 
		for s in size:
			num_features *= s 
		return num_features 

	def predict_proba(self, x):
		'''
		用于和其他学习器匹配，适用于整体框架
		'''
		transpose_array = x.T 
		input_tensor = torch.Tensor(transpose_array).view(1,3,64)
		good_num = float(self.forward(input_tensor)[0][1])
		good_proba = 1/(1+np.exp(-good_num))
		return np.array([[1-good_proba, good_proba]])

if __name__ == '__main__':
	num_of_training = 1000
	track_list = np.load("train_set/jsbachcoding_bach.npy").tolist()
	train_set, target_set = buiding_total_train_set(num_of_training, 64, track_list)
	X_train, X_test, y_train, y_test = train_test_split(\
		train_set, target_set, stratify=target_set, random_state=42)
	net = MusicNet()
	criterion = nn.CrossEntropyLoss()
	optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

	# 开始训练
	for epoch in range(50):
		print("epoch: {}".format(epoch))
		running_loss = 0.0
		batch_size = 50
		for i in range(int(1.5*num_of_training/batch_size)):
			optimizer.zero_grad()
			list_tensor = []
			label_list = []
			for j in range(batch_size):
				label = y_train[batch_size*i+j]
				label_list.append(label)
				tmp_array = X_train[batch_size*i+j]
				transpose_array = tmp_array.T
				tmp_tensor = torch.Tensor(transpose_array).view(1,3,64)
				list_tensor.append(tmp_tensor)
			label_tensor = torch.Tensor(label_list).reshape(batch_size).long()
			batch_tensor = torch.cat(list_tensor,0)
			output_tensor = net(batch_tensor)
			loss = criterion(output_tensor, label_tensor)
			loss.backward()
			optimizer.step()

			running_loss += loss.item()
			if (i == int(1.5*num_of_training/batch_size)-1):
				print("running_loss: {:.3f}".format(running_loss))
				running_loss = 0.0

	# 计算 准确率
	num_correct_train = 0
	for i in range(int(1.5*num_of_training)):
		label = y_train[i]
		tmp_array = X_train[i]
		transpose_array = tmp_array.T
		tmp_tensor = torch.Tensor(transpose_array).view(1,3,64)
		output = net(tmp_tensor)
		prediction = torch.max(output, 1)[1]
		pred_y = prediction.data.numpy().squeeze()
		if (pred_y == label[0]):
			num_correct_train += 1
	num_correct_test = 0
	for i in range(500):
		label = y_test[i]
		tmp_array = X_test[i]
		transpose_array = tmp_array.T
		tmp_tensor = torch.Tensor(transpose_array).view(1,3,64)
		output = net(tmp_tensor)
		prediction = torch.max(output, 1)[1]
		pred_y = prediction.data.numpy().squeeze()
		if (pred_y == label[0]):
			num_correct_test += 1
	print("Train Accuracy: {:3f}".format(num_correct_train/(1.5*num_of_training)))
	print("Test Accuracy: {:3f}".format(num_correct_test/(0.5*num_of_training)))

	# saving net data
	torch.save(net.state_dict(), 'models/music_net.tar')
