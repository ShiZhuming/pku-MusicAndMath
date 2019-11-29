#!/usr/bin/env python
# coding: utf-8

# In[1]:


import midi


# In[35]:


f=open('in.txt')
r=f.readlines()
f.close()
for i in range(len(r)):
    r[i]=r[i][:-1].split(' ')
    for j in range(len(r[i])):
        r[i][j]=int(r[i][j])
# print(r)


# In[37]:


pattern=midi.Pattern(format=1, resolution=480, tracks=[midi.Track(  [midi.TimeSignatureEvent(tick=0, data=[4, 2, 24, 8]),
   midi.KeySignatureEvent(tick=0, data=[0, 0]),
   midi.SetTempoEvent(tick=0, data=[7, 161, 32]),
   midi.ControlChangeEvent(tick=0, channel=0, data=[121, 0]),
   midi.ProgramChangeEvent(tick=0, channel=0, data=[0]),
   midi.ControlChangeEvent(tick=0, channel=0, data=[7, 100]),
   midi.ControlChangeEvent(tick=0, channel=0, data=[10, 64]),
   midi.ControlChangeEvent(tick=0, channel=0, data=[91, 0]),
   midi.ControlChangeEvent(tick=0, channel=0, data=[93, 0]),
   midi.PortEvent(tick=0, data=[0])])])
delta=0
for i in range(len(r)):
    if(len(r[i])==1):
        delta+=r[i][0]*240
        continue
    for j in range(1,len(r[i])):
        pattern[0].append(midi.NoteOnEvent(tick=delta, channel=0, data=[r[i][j], 80]))
        delta=0
    delta=r[i][0]*(240-12)-1
    for j in range(1,len(r[i])):
        pattern[0].append(midi.NoteOnEvent(tick=delta, channel=0, data=[r[i][j], 0]))
        delta=0
    delta=r[i][0]*12+1
pattern[0].append(midi.EndOfTrackEvent(tick=1, data=[]))
# print(pattern)


# In[39]:


midi.write_midifile("out.mid", pattern)


# In[ ]:




