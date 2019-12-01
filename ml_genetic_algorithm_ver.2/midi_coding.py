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
					tick = pattern[i][j+1].tick
					if (tick < 180):
						list_i.append([120, pattern[i][j].data[0], pattern[i][j].data[1]])
					elif (tick < 300):
						list_i.append([240, pattern[i][j].data[0], pattern[i][j].data[1]])
					else:
						list_i.append([360, pattern[i][j].data[0], pattern[i][j].data[1]])
			else:
				continue
		if not breaking_flag:
			array_tmp = np.array(list_i)
			pitch_mean = np.mean(array_tmp, axis=0)
			if (pitch_mean.shape == (3,) and pitch_mean[1] >= 60):
				list_track.append(list_i)
		breaking_flag = True
	return list_track

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

	for i in range(len(list_track_one)):
		track.append(mido.Message('note_on', note = list_track_one[i][1], velocity = list_track_one[i][2], time=50))
		track.append(mido.Message('note_off', note = list_track_one[i][1], velocity = list_track_one[i][1], time=list_track_one[i][0]))
	mid.save(filepath)

if __name__ == '__main__':

	filepath = "训练集/jsbach/train_midi28.MID"
	list_track = midi_conversion(filepath)
	print("track number for train_midi28.MID: {}".format(len(list_track)))
	reconversion(list_track[0], "test_midi_code.mid")

	track_list = []
	for i in range(1,153):
		try:
			filepath = "训练集/jsbach/train_midi" + str(i) + ".MID"
			list_track = midi_conversion(filepath)
			if (list_track != []):
				track_list.extend(list_track)
		except:
			continue
	print("track numbers for all midiFIle: {}".format(len(track_list)))

	track_array = np.array(track_list)
	# Saving list in np.array form, reopen it using np.load() method
	np.save("训练集/jsbachcoding_bach.npy", track_array)
	reload_track_array = np.load("训练集/jsbachcoding_bach.npy")
	# Test 
	print(reload_track_array == track_array)
