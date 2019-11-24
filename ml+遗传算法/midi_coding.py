#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author:Lin
# For coding midi file


import midi
import numpy as np
import mido

def midi_conversion(filepath):
	'''
	converting midi to number list by considering C4 as pitch 60
	'''
	pattern = midi.read_midifile(filepath)
	list_track = []
	current_program = 0
	breaking_flag = True
	for i in range(len(pattern)):
		list_i = []
		for j in range(len(pattern[i])):
			if (type(pattern[i][j]) == midi.events.ProgramChangeEvent):
				current_program = pattern[i][j].data[0]
				#print("In track {}, Program change to {}".format(i, pattern[i][j].data))
				if (current_program > 7):# Only collectiong the piano track
					breaking_flag = True
					break
				else:
					breaking_flag = False
			elif (type(pattern[i][j]) == midi.events.NoteOnEvent):
				if (pattern[i][j+1].tick != 0):# The begining of the melody cannot be 0
					list_i.append([pattern[i][j+1].tick, pattern[i][j].data[0]])
			else:
				continue
		if not breaking_flag:
			array_tmp = np.array(list_i)
			pitch_mean = np.mean(array_tmp, axis=0)
			if (pitch_mean.shape == (2,) and pitch_mean[1] >= 60):
				list_track.append(list_i)
		breaking_flag = True
	return list_track

def track_coding(list_track):
	'''
	convert picth-list to anoether form
	'''
	pitch_array = np.array(list_track[0])
	duration_mean = np.mean(pitch_array, axis=0)[0]
	#print("pitch array: {}".format(pitch_array))
	#print("duration mean: {}".format(duration_mean))
	new_list_track = []
	for i in range(len(list_track)):
		new_list_i = []
		for j in range(len(list_track[i])):
			# consider 2*mean, 3*mean as borders
			if (list_track[i][j][0] < 2*duration_mean):
				new_list_i.append(list_track[i][j][1])
			elif (list_track[i][j][0] < 4*duration_mean):
				new_list_i.append(list_track[i][j][1])
				new_list_i.append(0)
			else:
				new_list_i.append(list_track[i][j][1])
				new_list_i.append(0)
				new_list_i.append(0)
		new_list_track.append(new_list_i)
	return new_list_track

def reconversion(list_track_one, filepath):
	'''
	reconversing codes to midi file
	'''
	mid = mido.MidiFile()
	track = mido.MidiTrack()
	mid.tracks.append(track)
	print("\n----Making Midi File----\n")
	print("Ticks per beat:{}\n".format(mid.ticks_per_beat))
	track.append(mido.MetaMessage('set_tempo', tempo=500000, time=0))
	track.append(mido.MetaMessage('track_name', name='Piano', time=0))
	track.append(mido.Message('program_change', program=0, time=0))

	current_pitch = list_track_one[0]
	i = 0
	while i in range(0, len(list_track_one)):
		if (list_track_one[i] != 0):
			current_pitch = list_track_one[i]
			track.append(mido.Message('note_on', note = current_pitch, velocity = 96, time=120))
			track.append(mido.Message('note_off', note = current_pitch, velocity = 96, time=120))
			i += 1
		else:
			current_time = 120
			while (i < len(list_track_one) and list_track_one[i] == 0 ):
				current_time += 120
				i += 1
			track.append(mido.Message('note_on', note = current_pitch, velocity = 96, time=120))
			track.append(mido.Message('note_off', note = current_pitch, velocity = 96, time=current_time))
	mid.save(filepath)

if __name__ == '__main__':

	filepath = "训练集/jsbach/train_midi28.MID"
	code_list = track_coding(midi_conversion(filepath))
	print("track number for train_midi28.MID: {}".format(len(code_list)))
	reconversion(code_list[0], "test_midi_code.mid")

	track_list = []
	for i in range(1,153):
		try:
			filepath = "训练集/jsbach/train_midi" + str(i) + ".MID"
			list_track = midi_conversion(filepath)
			if (list_track != []):
				code_list = track_coding(list_track)
				track_list.extend(code_list)
		except:
			continue
	print("track numbers for all midiFIle: {}".format(len(track_list)))

	track_array = np.array(track_list)
	# Saving list in np.array form, reopen it using np.load() method
	np.save("训练集/jsbachcoding_bach.npy", track_array)
	reload_track_array = np.load("训练集/jsbachcoding_bach.npy")
	# Test 
	print(reload_track_array == track_array)
