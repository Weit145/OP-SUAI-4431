def qs(arr:list)->list:
    if (len(arr)<1):
        return arr
    target = arr[len(arr)//2]
    left = [x for x in arr if x<target]
    right = [x for x in arr if x>target]
    middele = [x for x in arr if x==target]
    return qs(left)+middele+qs(right)


def sliv(arr1:list, arr2:list)->list:
    arr=[]
    a=b=0
    while len(arr1)!=a and len(arr2)!=b:
        if arr1[a]<=arr2[b]:
            arr.append(arr1[a])
            a=a+1
        elif arr2[b]<arr1[a]:
            arr.append(arr2[b])
            b=b+1
    if b==len(arr2):
        arr.extend(arr1[a:])
    elif a==len(arr1):
        arr.extend(arr2[b:])
    return arr



def merg(arr:list)->list:
    if (len(arr)<=1):
        return arr
    target = len(arr)//2
    left = arr[:target]
    right = arr[target:]
    return sliv(merg(left),merg(right))


print(merg(arr=[3,2,1,4,5]))