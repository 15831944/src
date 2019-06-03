#!/usr/bin/python
# -*- coding: UTF-8 -*-

from PIL import Image
import numpy
import os
import itchat

def ProgressPrint(cur_len, sec_count, total_len):
	sec_len = int(total_len/sec_count)
	if cur_len % sec_len == 0:
		print ('[+]Progress %3.2f%%'%(cur_len/total_len*100))
	elif cur_len >= total_len:
		print ('[+]Progress %3.2f%%'%(100))


def paste_img(host_im, img_path, x, y):
	try:
		im = Image.open(img_path).convert('RGB')
		im = im.resize((PASTE_IMG_SIZE, PASTE_IMG_SIZE))
		host_im.paste(im, (x, y))
		return True
	except:
		return False


def get_img_matrix(img_path):
	im = Image.open(img_path).convert('L')
	data_object = im.getdata()
	[row_len, col_len] = im.size
	return numpy.array(list(data_object)).reshape(col_len, row_len)


def generate_imgimg(character_img_matrix, paste_img_path_list):
	height = character_img_matrix.shape[0]
	width = character_img_matrix.shape[1]

	img_img = Image.new('RGB', (width * PASTE_IMG_SIZE, height * PASTE_IMG_SIZE), 0xFFFFFF)

	img_num = 0
	use_count = 0

	L_WHITE = 0xff
	for i in range(0, height):
		for j in range(0, width):
			if character_img_matrix[i][j] != L_WHITE:

				while(False == paste_img(img_img, paste_img_path_list[img_num], j * PASTE_IMG_SIZE, i * PASTE_IMG_SIZE)):
					print ('[-]Err invalid img-%s'%(paste_img_path_list[img_num]))
					img_num += 1
					img_num %= len(paste_img_path_list)
				
				img_num += 1
				img_num %= len(paste_img_path_list)
				use_count += 1

		ProgressPrint(i, 5, height)

	ProgressPrint(height, 5, height)

	print ('[*]Final use %d photos in total'%(use_count))
	return img_img


def get_imgpath_list(img_dir):
	img_path_list = []
	for roots, dirs, files in os.walk(img_dir):
		for filename in files:
			img_path_list.append(os.path.join(roots, filename))
	return img_path_list


def get_wechat_profile_photo(save_dir):
	itchat.auto_login()

	try:
		#获取微信好友信息列表
		friendList = itchat.get_friends(update=True)
		friend_count = len(friendList)
		print ('[+]You have %d friends in all'%(friend_count))

		#读取好友头像
		photo_count = 0
		ProgressPrint(photo_count, 5, friend_count)
		for friend in friendList:
			friend['head_img'] = itchat.get_head_img(userName=friend['UserName'])
			friend['head_img_name'] = "%s\\%d.jpg" % (save_dir, photo_count)
			#写入文件
			with open(friend['head_img_name'],'wb') as f:
				f.write(friend['head_img'])

			photo_count += 1
			ProgressPrint(photo_count, 5, friend_count)

	except:
		print ('[-]Err occured when get friendphotos')

	itchat.logout()
#---------------------------------------------------------

character_img_path = input("input the character img path: ")
img_dir = input("input the profile photo dir path: ")
#save_path = input("input the save path: ")
PASTE_IMG_SIZE = 100


print ('[*]Please wait for wechat preparing...')
get_wechat_profile_photo(img_dir)

print ("[*]IMG generation begin...")
paste_img_path_list = get_imgpath_list(img_dir)
character_img_matrix = get_img_matrix(character_img_path)
new_img = generate_imgimg(character_img_matrix, paste_img_path_list)
new_img.show()
#new_img.save(save_path)