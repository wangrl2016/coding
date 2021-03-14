#!/usr/bin/env python3


class heap:
    @staticmethod
    def insert(nums, x):
        # 将元素加入到堆的末尾位置
        nums.append(x)
        idx = len(nums) - 1
        while idx != 0:
            parent_idx = int((idx - 1) / 2)
            # 如果插入的元素小，则需要和父节点交换位置
            if nums[idx] < nums[parent_idx]:
                nums[idx], nums[parent_idx] = nums[parent_idx], nums[idx]
                idx = parent_idx
            else:
                break

    @staticmethod
    def delete(nums):
        # -1表示数组中最后一个元素
        # 最后一个元素和最开始元素交换
        nums[0], nums[-1] = nums[-1], nums[0]

        res = nums.pop(-1)
        lens, idx = len(nums), 0
        while True:
            temp = idx
            left = idx * 2 + 1
            right = idx * 2 + 2
            # 查找左子树最小值
            if left < lens and nums[idx] > nums[left]:
                idx = left
            if right < lens and nums[idx] > nums[right]:
                idx = right
            if idx == temp:
                break
            else:
                nums[idx], nums[temp] = nums[temp], nums[idx]

        return res


if __name__ == '__main__':
    arr = [6, 8, 9, 1, 3, 5, 4, 3, 2, 7]
    # 堆的存储位置
    result = []

    for a in arr:
        heap.insert(result, a)
    print(result)

    arr.clear()
    while result:
        arr.append(heap.delete(result))
    print(arr)
