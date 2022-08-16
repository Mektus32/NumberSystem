import tkinter as tk
import os


def calculate(a1, a2, a3, a4, n, last, text):
    tmp = f'./NumberSystem a1={a1.get()} a2={a2.get()} a3={a3.get()} n={n.get()} last={last.get()}'
    if a4.get() == '-1':
        result = os.system(f'./NumberSystem a1={a1.get()} a2={a2.get()} a3={a3.get()} n={n.get()} last={last.get()}')
    else:
        result = os.system(
            f'./NumberSystem a1={a1.get()} a2={a2.get()} a3={a3.get()} a4={a4.get()} n={n.get()} last={last.get()}')
    if not result:
        with open('output.txt', 'r') as file:
            i = 1.0
            text.delete('1.0', tk.END)
            for line in file.readlines():
                text.insert(i, line)
                i += 1


window = tk.Tk()
window.geometry("700x520")

tk.Label(window, text="a1").grid(row=0, column=0)
a1 = tk.Entry(window)
a1.insert(0, '1')
a1.grid(row=0, column=1)

tk.Label(window, text="a2").grid(row=1, column=0)
a2 = tk.Entry(window)
a2.insert(0, '2')
a2.grid(row=1, column=1)

tk.Label(window, text="a3").grid(row=2, column=0)
a3 = tk.Entry(window)
a3.insert(0, '3')
a3.grid(row=2, column=1)

tk.Label(window, text="a4").grid(row=3, column=0)
a4 = tk.Entry(window)
a4.insert(0, '-1')
a4.grid(row=3, column=1)

tk.Label(window, text="n").grid(row=4, column=0)
n = tk.Entry(window)
n.insert(0, '100')
n.grid(row=4, column=1)

tk.Label(window, text="last").grid(row=5, column=0)
last = tk.Entry(window)
last.insert(0, '2')
last.grid(row=5, column=1)

tk.Label(window, text="Result").grid(row=6, column=0)
text = tk.Text(window)
text.grid(row=6, column=1)

tk.Button(window, text="Calculate", command=lambda: calculate(a1, a2, a3, a4, n, last, text)).grid(row=7,
                                                                                                   column=0,
                                                                                                   columnspan=2)

window.mainloop()
