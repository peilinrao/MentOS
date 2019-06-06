
#define ASM     1
#include "idt_jmp.h"

.text

.globl IDT_JMP_TBL, IRQ_ASM_LNK
.globl do_irq

# IDT_JMP_TBL:
#		DESCRIPTION: entries aligned to IDT. Push all registers and push irq number
IDT_JMP_TBL:
	.align 16
	pushal
	pushl	$-1
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-2
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-3
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-4
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-5
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-6
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-7
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-8
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-9
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-10
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-11
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-12
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-13
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-14
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-15
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-16
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-17
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-18
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-19
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-20
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-21
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-22
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-23
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-24
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-25
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-26
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-27
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-28
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-29
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-30
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-31
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-32
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-33
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-34
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-35
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-36
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-37
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-38
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-39
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-40
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-41
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-42
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-43
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-44
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-45
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-46
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-47
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-48
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-49
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-50
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-51
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-52
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-53
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-54
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-55
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-56
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-57
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-58
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-59
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-60
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-61
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-62
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-63
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-64
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-65
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-66
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-67
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-68
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-69
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-70
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-71
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-72
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-73
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-74
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-75
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-76
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-77
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-78
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-79
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-80
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-81
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-82
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-83
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-84
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-85
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-86
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-87
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-88
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-89
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-90
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-91
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-92
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-93
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-94
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-95
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-96
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-97
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-98
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-99
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-100
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-101
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-102
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-103
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-104
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-105
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-106
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-107
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-108
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-109
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-110
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-111
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-112
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-113
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-114
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-115
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-116
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-117
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-118
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-119
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-120
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-121
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-122
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-123
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-124
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-125
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-126
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-127
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-128
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-129
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-130
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-131
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-132
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-133
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-134
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-135
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-136
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-137
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-138
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-139
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-140
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-141
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-142
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-143
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-144
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-145
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-146
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-147
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-148
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-149
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-150
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-151
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-152
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-153
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-154
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-155
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-156
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-157
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-158
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-159
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-160
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-161
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-162
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-163
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-164
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-165
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-166
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-167
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-168
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-169
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-170
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-171
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-172
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-173
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-174
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-175
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-176
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-177
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-178
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-179
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-180
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-181
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-182
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-183
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-184
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-185
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-186
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-187
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-188
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-189
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-190
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-191
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-192
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-193
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-194
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-195
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-196
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-197
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-198
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-199
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-200
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-201
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-202
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-203
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-204
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-205
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-206
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-207
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-208
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-209
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-210
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-211
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-212
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-213
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-214
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-215
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-216
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-217
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-218
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-219
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-220
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-221
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-222
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-223
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-224
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-225
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-226
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-227
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-228
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-229
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-230
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-231
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-232
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-233
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-234
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-235
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-236
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-237
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-238
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-239
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-240
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-241
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-242
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-243
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-244
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-245
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-246
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-247
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-248
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-249
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-250
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-251
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-252
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-253
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-254
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-255
	jmp		IRQ_ASM_LNK
	.align 16
	pushal
	pushl	$-256
	jmp		IRQ_ASM_LNK

# IRQ_ASM_LNK:
# 		DESCRIPTION: call do_irq, tear down the stack. Restore register values
.align 16
IRQ_ASM_LNK:
	call  do_irq
	add   $4, %esp
	popal
	iret
