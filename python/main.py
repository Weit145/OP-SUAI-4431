# import math 
# l="abcde"
# hobby=[]
# for i in l:
#     hobby.append(i)
# print(hobby)

# l=[1, 2, 3, 4, 5, 6]
# print(l[2:-1])

# dict={
# 	'year' : '2025',
# 	'month': '12',
# 	'day'  : '31',
# }

# print(dict['year']+'-'+dict['month']+'-'+dict['day'])

# for i in range(1,101):
#     print(i)

# for i in range(-100,0):
#     print(i)

# for i in range(0,100):
#     print(100-i)

# for i in range(1,101):
#     if i%2==0:print(i)
    
# for i in range(1,101):
#     if i%3==0:print(i)

# l=[1, 2, 3, 4, 5, 6]
# print(l[-2:])

# l='abcdeabc'
# hobby=[]
# for i in l:
#     if i not in hobby:hobby.append(i)
# print(hobby)

# def zad1_5_1():
#     sum=0
#     for i in range(101):
#         sum+=i
#     return sum

# def zad1_5_2():
#     sum=0
#     for i in range(101):
#         if i%2==0: sum+=i
#     return sum

# def zad1_5_3():
#     sum=0
#     for i in range(101):
#         if i%2!=0:sum+=i
#     return sum

# def zad1_5_4(a=3,b=2):
#     return a%b

# def zad1_5_5(l=[1, 2, 3, 4, 5, 6]):
#     return l[::2]



# print(zad1_5_5())



# def zad1_6_1(l=[1, 2, 3, 4, 5]):
#     sum=0
#     for i in l:
#         sum+=i
#     return sum


# def zad1_6_2(l=[1, 2, 3, 4, 5]):
#     sum=0
#     for i in l:
#         sum=i*i+sum
#     return sum

# def zad1_6_3(l=[1, 2, 3, 4, 5]):
#     sum=0
#     for i in l:
#         sum=i*i+sum
#     return math.sqrt(sum)

# def zad1_6_4(l=[1, 2, -3, 4, -5]):
#     sum=0    
#     for i in l:
#         if i>=0: sum+=i
#     return sum

# def zad1_6_5(l=[-1, 2, -3, 4, 5, 11]):
#     sum=0
#     for i in l:
#         if i>0 and i<10:sum+=i
#     return sum

# def zad1_6_6(l='abcde'):
#     k=1
#     for i in range(len(l)):
#         print(l[-i-1],k)
#         k+=1

# zad1_6_6()



# def zad8_12_1(read,n,write):
#     l=""
#     t=""
#     cout=0
#     for i in read:
#         for k in i:
#             if k==' ':cout+=1
#             if cout==n: 
#                 read.close()
#                 write.close()
#                 return
#             write.write(k)
#     read.close()
#     write.close()
# zad8_12_1(open('r.txt'),2,open('w.txt','w'))

# dict={
# 	'a': 1,
# 	'b': 2,
# 	'c': 3, 
# 	'd': 4,
# }
# def zad1_7_1(dict):
#     sum=0
#     for i in dict:
#         sum+=dict[i]
#     return sum

# def zad1_7_2(dict):
#     sum=0
#     for i in dict:
#         sum+=dict[i]*dict[i]
#     return sum

# def zad1_7_3(n='abcde'):
#     l=[]
#     for i in n:
#         l.append(i)
#     return l

# def zad1_7_4(n=12345):
#     l=[]
#     for i in str(n):
#         l.append(i)
#     return l

# def zad1_7_5(n=12345):
#     l=""
#     k=str(n)
#     for i in range(len(k)):
#         l+=k[-i-1]
#     return l

# def zad1_7_6(n=12345):
#     sum=0
#     for i in str(n):
#         sum+=int(i)
#     return sum


# print(zad1_7_6())




# def zad1_8_1(l=(1, 2, 3, 4, 5)):
#     sum=0
#     for i in l:
#         sum+=i
#     return sum

# def zad1_8_2(l=[1, 2, 3, 4, 5, 6]):
#     for i in range(len(l)):
#         l[i]=l[i]*1.1
#     return l 

# def zad1_8_3(l='abcdef'):
#     return l[:3]

# def zad1_9_1(l='abcdef'):
#     return l[-3:]

# def zad1_9_2():
#     dict={
#         'a': 1,
# 	    'b': 2,
# 	    'c': 3, 
# 	    'd': 4,
#     }
#     for i in dict:
#         dict[i]*=2
#         print(dict[i])
#     return

# def zad1_10_1(l='abcdef'):
#     return l[::2]

# def zad1_10_2(l=12345):
#     t=str(l)
#     for i in range(len(t)):
#         print(t[-i-1])
#     return 
# print(zad1_10_2())


# def zad2_1_1(l=["http://","http://sadasd","sadas"]):
#     for i in l:
#         if "http://" in i:continue
#         l.remove(i)
#     return l

# def zad2_1_2(l="0111111110"):
#     cout=0
#     for i in l:
#         if i=="0": return cout
#         cout+=1

# def zad2_1_3(l=[1,2,3,4,1],n=1):
#     for i in l:
#         if i==n: l.remove(i)
#     return l

# def zad2_1_4():
#     for i in range(10,1001):
#         t=str(i)
#         if int(t[0])+int(t[1])==5:print(i)
#     return

# def zad2_1_5(l='abcdeabc'):
#     t=''
#     for i in l:
#         if i not in t:t+=i
#     return t

# print(zad2_1_5())


# def zad2_2_1(l=[1,1,1,-1,-1,1]):
#     sum=0
#     for i in l:
#         if i<0:sum+=1
#     return sum

# def zad2_2_2(l=[1,1,1,-1,-1,1]):
#     t=[]
#     for i in l:
#         if i>=0:t.append(i)
#     return t

# def zad2_2_3(l="1222222243"):
#     t=l[:-2]+l[-1]
#     return t

# def zad2_2_4(l=["123.html",".html","213"]):
#     t=[]
#     for i in l:
#         if i[-5:]==".html":t.append(i)
#     return t

# def zad2_2_5(l=[1.456, 2.125, 3.32, 4.1, 5.34]):
#     t=[]
#     for i in l:
#         t.append(int(i))
#     return t


# def zad2_2_6():
#     dict={
#         'a': 1,
# 	    'b': 2,
# 	    'c': 3, 
# 	    'd': 4,
#     }
#     t=[]
#     for i in dict:
#         t.append(dict[i])
#     return t


# print(zad2_2_6())


# zad2_3_1=lambda l="123",k="321":l[:1]==k[-1]

# def zad2_3_2(l="00010"):
#     cout=3
#     for i in range(len(l)):
#         if l[i]=="0": cout-=1
#         if cout==0:return i+1


# def zad2_3_3(l='12,34,56'):
#     sum=0
#     for i in l.split(","):
#         sum+=int(i)
#     return sum

# def zad2_3_4(l='2025-12-31'):
#     dict={
#         'year' : '',
# 	    'month': '',
# 	    'day'  : '',
#     }
#     cout=0
#     f=l.split("-")
#     for i in dict:
#         dict[i]=f[cout]
#         cout+=1
#     return dict

# def zad2_3_5():
#     dict={
# 	'a': 1,
# 	'b': 2,
# 	'c': 3, 
# 	'd': 4,
#     }
#     l=set('')
#     for i in dict:
#         l.add(dict[i])
#     return l

# print(zad2_3_5())


# def zad2_4_1(l="d2d"):
#     t=('0','1','2','3','4','5','6','7','8','9')
#     for i in range(len(l)):
#         if l[i] in t:return i
#     return

# def zad2_4_2(a=21436):
#     l=str(a)
#     for i in l:
#         if int(i)%2==0:print (i)


# def zad2_4_3(dict={'a': 1,'b': 2,'c': 3, 'd': 4,}):
#     l=[]
#     for i in dict:
#         l.append(i)
#     return l

# def zad2_4_4(l='abcde'):
#     t=""
#     cout=0
#     for i in l:
#         if cout%2==0: t+=i.upper()
#         else: t+=i
#         cout+=1
#     return t

# def zad2_4_5(l='aaa bbb ccc'):
#     flag=True
#     t=""
#     for i in l:
#         if flag and i!=" ":t+=i.upper();flag=False
#         elif i==" ":flag=True;t+=i
#         else:t+=i
#     return t


# def zad2_4_6(l='2025-12-31'):
#     t=tuple(l.split('-'))
#     return t
    


# print(zad2_4_6())



# def zad2_5_1(l='023m0df0dfg0'):
#     t=set()
#     for i in range(len(l)):
#         if l[i]=="0":t.add(i)
#     return set(t)

# def zad2_5_2(l='abcdefg'):
#     t=""
#     for i in range(len(l)):
#         if (i+1)%3==0:continue
#         t+=l[i]
#     return t



# def zad2_5_3(l=['2025', '12', '31']):
#     l.reverse()
#     return tuple(l)

# print(zad2_5_3())


# def zad2_6_1(l="g22l2"):
#     t={"1","2","3","4"}
#     k=[]
#     for i in range(len(l)):
#         if l[i] in t:k.append(i)
#     return k

# def zad2_6_2(l='AbCdE'):
#     t=''
#     for i in l:
#         if i==i.upper(): t+=i.lower()
#         else: t+=i.upper()
#     return t

# def zad2_6_3(l=[1, 2, 3, 4, 5, 6]):
#     t=[]
#     k=""
#     for i in range((len(l))):
#         if (i+1)%2!=0:
#             k=str(l[i])
#         else:
#             k+=str(l[i])
#             t.append(int(k))
#     return t

# def zad2_6_4(l='aaa bbb ccc eee fff'):
#     cot=1
#     flag=0
#     t=''
#     for i in l:
#         if i==" ": flag=1; t+=i;cot+=1
#         elif flag==1 and cot%2==0: t+=i.upper(); flag=0
#         else: t+=i
#     return t



# print(zad2_6_4())


# def zad2_7_1(l='W'):
#     if l==l.upper(): print("Up")
#     else: print("Down")
#     return

# def zad2_7_2(l=123789):
#     t=str(l)
#     k=""
#     for i in t:
#         if int(i)%2!=0: continue
#         k+=i
#     return int(k)


# def zad2_7_3(l=('2025', '12', '31')):
#     dict={
#         'year' : '',
# 	    'month': '',
# 	    'day'  : '', 
#     }
#     cot=0
#     for i in dict:
#         dict[i]=l[cot]
#         cot+=1
#     return dict        

# print(zad2_7_3())


# def zad2_8_1():
#     l=[]
#     c=1
#     for i in range(10):
#         l.append(c)
#         c+=1
#     return l

# def zad2_8_2(l="AAffa"):
#     c=0
#     for i in l:
#         if i==i.upper():c+=1
#     if c>2:return None
#     return "Good"

# def zad2_8_3(l=['1', '2', '3', '4', '5']):
#     for i in range(len(l)):
#         l[i]=int(l[i])
#     return l

# def zad2_9_1():
#     l=[]
#     for i in range(1,101):
#         if i%2==0:l.append(i)
#     return l

# def zad2_9_2(l=[1, 2, 3, 4, 5]):
#     t=l
#     t.reverse()
#     return t

# def zad2_9_3(txt1 = 'abcde',txt2 = '12345'):
#     if len(txt1)!=len(txt2):return None
#     dic={txt1[i]:txt2[i] for i in range(len(txt1))}
#     return dic

# def zad2_10_1(l="123ff123"):
#     t=['1', '2', '3', '4', '5','6','7','8','9','0']
#     c:int=0
#     for i in l:
#         if i not in t:
#             c+=1
#     if c>2: return None
#     return "Good"

# def zad2_10_2(l=1122345):
#     '''Получает первую четную цифру с конца.
    
#     Args:
#         l(int):Число с которого получим цифру
        
#     Returns:
#         int: Чётная цифра
#         None: нет таких
    
#     Examples:
#         >>>zad2_10_2(l=1122345)
#         4
#         >>>zad2_10_2(l=11)
#         None
#     '''
#     t=str(l)
#     for i in t[::-1]:
#         if int(i)%2==0:return int(i)
#     return None


# zad2_10_3=lambda l='abcde abcde abcde': l.replace(l[0],"!")

# def zad2_10_4(l=['1', '2', '3', '4', '5'])-> int:
#     sum=0
#     for i in l:
#         sum+=int(i)
#     return sum


# print(zad2_10_4())

# def zad3_1_1(lst=[])-> list:
#     lst=[i for i in range(1,7)]
#     return lst

# def zad3_1_2(lst1 = [1, 2, 3],lst2 = [4, 5, 6])-> list:
#     return lst1+lst2

# def zad3_1_3(lst=[1, 2, 3, 4, 5, 6, 1, 7])->int:
#     sum=0
#     for i in range(len(lst)//2):
#         sum+=lst[i]
#     return sum


# print(zad3_1_3())

def zad3_2_1()-> list:
    lst=[i for i in range(1,10,2)]
    return lst


def zad3_2_2(tpl1 = (1, 2, 3),tpl2 = (4, 5, 6))-> tuple:
    tpl=tpl1+tpl2
    return tpl


def zad3_2_3(dct1 = {'a': 1,'b': 2,},dct2 = {'c': 3, 'd': 4,})-> dict:
    dic=dct1|dct2
    return dic

def zad3_2_4(l=[1, 2, 3, 4, 5, 6])->int:
    sum1=0
    sum2=0
    for i in range(len(l)//2):
        sum1+=l[i]
        sum2+=l[-i-1]
    return sum1/sum2


print(zad3_2_4())















def qsort(lst:list)->list:
    if len(lst)<=1:
        return lst
    else:
        pivor=lst[(len(lst)//2)]
        left=[i for i in lst if i<pivor]
        right=[i for i in lst if i>pivor]
        midl=[i for i in lst if i==pivor]
        return qsort(left)+midl+qsort(right)
    



'''
    ruff check .py
    ruff --fix .py
    ДОКУМЕНТИРОАТЬ КЛАССЫ И ФУНКЦИИ
    И ЗАПОМНИ ТЫ ЛЕГЕНДА ЕСЛИ НЕ ТЫ ТО НИКТО ДРУГОЙ СТАНЬ ПЕРВЫМ КТО ЭТО СДЕЛАТЬ ВЕДЬ ТЫ ТОТ КТО ЭТОГО ДОСТОИН ДОКАЖИ ИМ ВСЕМ ЭТИ СЛОА КОТОРЫЕ ТЫ ГОВОРИШЬ СЕБЕ. Я ЗНАЮ ТОЛЬКО ДВУХ ТАКИХ ЛЮДЕЙ И Я С НИМ РОДИЛСЯ В ОДИН ДЕНЬ))))
'''