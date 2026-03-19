# 🧾 Linux & Bash Commands Cheat Sheet

## 1. Basic Navigation Commands

| Command | Description | Example |
| --- | --- | --- |
| `pwd` | Show current directory | `pwd` |
| `ls` | List files | `ls` |
| `ls -l` | Detailed list (permissions, owner, size) | `ls -l` |
| `ls -a` | Show hidden files | `ls -a` |
| `cd dir` | Change directory | `cd Desktop` |
| `cd ..` | Go to parent directory | `cd ..` |
| `cd ~` | Go to home directory | `cd ~` |
| `clear` | Clear terminal | `clear` |

📌 Notes

- `.` current dir
- `..` parent dir
- `/` root directory
- Linux has **single root hierarchy**

## 2. File & Directory Commands

| Command | Description |
| --- | --- |
| `touch file` | create file |
| `mkdir dir` | create directory |
| `rmdir dir` | remove empty dir |
| `rm file` | delete file |
| `rm -r dir` | delete directory |
| `cp a b` | copy |
| `mv a b` | move / rename |
| `file name` | show file type |
| `stat file` | detailed file info |

Example

```bash
mkdir test
touch a.txt
cp a.txt b.txt
mv b.txt c.txt
rm c.txt
```

## 3. File Viewing Commands

| Command | Description |
| --- | --- |
| `cat file` | show file |
| `less file` | scroll view |
| `head file` | first 10 lines |
| `head -n 5 file` | first 5 |
| `tail file` | last 10 |
| `tail -n 5 file` | last 5 |
| `wc file` | count lines/words |

Example

```bash
head -n 3 file.txt
tail -n 2 file.txt
```

## 4. Writing to Files

| Command | Meaning |
| --- | --- |
| `>` overwrite |  |
| `>>` append |  |

Example

```bash
echo hello > file.txt
echo world >> file.txt
```

📌 overwrite clears file first

## 5. Permissions

Format

```bash
rwx rwx rwx
u   g   o
```

| Symbol | Meaning |
| --- | --- |
| r | read |
| w | write |
| x | execute |

Numbers

| Value | Meaning |
| --- | --- |
| 4 | r |
| 2 | w |
| 1 | x |
| 7 | rwx |
| 6 | rw- |
| 5 | r-x |

Examples

```bash
chmod 777 file
chmod 755 file
chmod u+x file
chmod g-w file
```

📌 u user, g group, o others, a all

## 6. Search & Filter Commands

### grep

```bash
grep text file
grep -i text file
grep -n text file
```

Patterns

| Pattern | Meaning |
| --- | --- |
| ^A | start with A |
| A$ | end with A |
| [abc] | one of |
| [0-9] | range |
| [^0-9] | not |
| . | any char |
| * | repeat |
| {2,5} | range repeat |

Example

```bash
grep ^T file
grep -i hello file
```

📌 regex patterns from lab

## 7. Pipes & Filters

| Command | Meaning |
| --- | --- |
| `sort` | sort |
| `uniq` | remove duplicates |
| `tr` | replace chars |
| `paste` | merge |
| `seq` | numbers |

Examples

```bash
cat f | sort
cat f | uniq
cat f | tr a z
seq 1 2 10
```

## 8. System Commands

| Command | Meaning |
| --- | --- |
| `man cmd` | manual |
| `sudo cmd` | root |
| `whoami` | user |
| `date` | date |
| `history` | history |
| `top` | processes |
| `exit` | exit |

## 9. Bash Script Basics

Create script

```bash
touch file.sh
chmod +x file.sh
./file.sh
```

Start script

```bash
#!/bin/bash
echo Hello
```

📌 must add shebang

## 10. Variables

```bash
x=5
name="Ali"

echo $x
echo $name
```

❌ wrong

```bash
x = 5
```

✔ no spaces

## 11. Arithmetic

```bash
y=$((y+1))
let y=y+1
expr $y + 1
```

Power

```bash
$((2**3))
```

## 12. If Statement

```bash
if [ a = b ]
then
echo yes
else
echo no
fi
```

Operators

| op | meaning |
| --- | --- |
| -eq | = |
| -ne | ≠ |
| -gt | > |
| -lt | < |
| -ge | >= |
| -le | <= |

## 13. Loops

### for

```bash
for i in 1 2 3
do
echo $i
done
```

```bash
for i in {1..5}
do
echo $i
done
```

### while

```bash
i=0
while [ $i -lt 5 ]
do
echo $i
i=$((i+1))
done
```

### until

```bash
until [ $i -gt 5 ]
do
echo $i
done
```

## 14. Arrays

```bash
arr=(a b c)

echo ${arr[0]}
echo ${arr[*]}
```

## 15. Functions

```bash
function f {
echo hi
}

f
```

With arg

```bash
function add {
echo $1
}

add 5
```

## 16. Useful Shortcuts

| Key | Meaning |
| --- | --- |
| ↑ | history |
| Ctrl + C | stop |
| Ctrl + L | clear |
| Tab | autocomplete |
| Ctrl + D | exit |

## 17. Extra Commands

| Command | Use |
| --- | --- |
| `find` | search files |
| `df -h` | disk |
| `du -h` | folder size |
| `ps` | processes |
| `kill` | stop process |
| `tar` | archive |
| `zip` | zip |
| `unzip` | unzip |
| `wget` | download |
| `nano` | editor |
| `vim` | editor |
