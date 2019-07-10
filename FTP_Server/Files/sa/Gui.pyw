from tkinter import filedialog
from tkinter import *
import tkinter.ttk

#Create Window
master = Tk()
master.configure(bg = "lightgrey")
master.title("DatenBank")
master.resizable(False, False)

#Create tree with scrollbar
lbf = Frame()
lbf.grid(row = 0, column = 0, padx = 10, pady = 10, columnspan = 6)

columnNames = ["Marke", "Typ", "Baujahr"]
dataList = tkinter.ttk.Treeview(lbf, column = columnNames, show = "headings")
dataList.pack(side = LEFT, fill = Y)

scroll = Scrollbar(lbf)
scroll.pack(side = RIGHT, fill = Y)

scroll.config(command = dataList.yview)
dataList.config(yscrollcommand = scroll.set)

dataList.bind("<Double-1>", lambda e: View())

for name in columnNames:
	dataList.heading(name, text = name)

#Save state
class FileManager:
	fileName = ""
	wasEdited = False

	@staticmethod
	def saveAsFile():
		file = filedialog.asksaveasfilename(initialdir = '/', title = "Select file", filetypes = (("csv files", "*.csv"), ("all files", "*.*")), defaultextension = ".csv")
		if not file:
			return

		FileManager.fileName = file
		FileManager.saveFile()

	@staticmethod
	def saveFile():
		with open(FileManager.fileName, 'w') as f:
			for ID in dataList.get_children():
				f.writelines(';'.join(dataList.item(ID)["values"]) + '\n')

	@staticmethod
	def openFile():
		file = filedialog.askopenfilename(initialdir = '/', title = "Select file", filetypes = (("csv files", "*.csv"), ("all files", "*.*")))
		if not file:
			return

		dataList.delete(*dataList.get_children())

		FileManager.fileName = file
		with open(FileManager.fileName, 'r') as f:
			for i in f.readlines():
				dataList.insert('', "end", values = i[:-1].split(';'))

#Base Classes
class PopUp:
	def __init__(self, title, bg):
		self.window = Toplevel()
		self.window.configure(bg = bg)
		self.window.title(title)
		self.window.resizable(False, False)
		self.window.grab_set()

		self.f = Frame(self.window)
		self.f.grid(row = 0, column = 0, pady = 5, padx = 5, columnspan = 2)

	def createButtons(self, text1, text2, command1, command2):
		Button(self.window, text = text1, width = 8, command = command1).grid(row = 1, column = 0, padx = 10, pady = 10)
		Button(self.window, text = text2, width = 8, command = command2).grid(row = 1, column = 1, padx = 10, pady = 10)

	def destroy(self):
		self.window.destroy()

class EntryPopUp(PopUp):
	def __init__(self, title, bg):
		PopUp.__init__(self, title, bg)

		Label(self.f, text = "Model", width = 10).grid(row = 0, column = 0)
		self.field1 = Entry(self.f)
		self.field1.grid(row = 0, column = 1, padx = 5)

		Label(self.f, text = "Typ", width = 10).grid(row = 1, column = 0)
		self.field2 = Entry(self.f)
		self.field2.grid(row = 1, column = 1, padx = 5)

		Label(self.f, text = "Baujahr", width = 10).grid(row = 2, column = 0)
		self.field3 = Entry(self.f)
		self.field3.grid(row = 2, column = 1, padx = 5)

#Derived Classes
class View(PopUp):
	def __init__(self):
		PopUp.__init__(self, "View", "lightgrey")

		for val in dataList.item(dataList.selection()[0])["values"]:
			Label(self.f, text = val, width = 21).pack(fill = BOTH)

		self.createButtons("OK", "Cancel", self.destroy, self.destroy)
		
class Search(EntryPopUp):
	def __init__(self):
		EntryPopUp.__init__(self, "Search", "lightgrey")
		self.createButtons("OK", "Cancel", self.search, self.destroy)

	def search(self):
		toFind = [self.field1.get(), self.field2.get(), self.field3.get()]
		for ID in dataList.get_children():
			if dataList.item(ID)["values"] == toFind:
				dataList.selection_set(ID)
				break

class Insert(EntryPopUp):
	def __init__(self):
		EntryPopUp.__init__(self, "Insert", "lightgrey")
		self.createButtons("OK", "Cancel", self.insert, self.destroy)

	def insert(self):
		dataList.insert('', "end", values = [self.field1.get(), self.field2.get(), self.field3.get()])
		FileManager.wasEdited = True

class Close(PopUp):
	def __init__(self):
		PopUp.__init__(self, "Save", "lightgrey")
		Label(self.f, text = "Save changes?", width = 21).pack(fill = BOTH)
		self.createButtons("Yes", "No", lambda: (FileManager.saveFile() if FileManager.fileName else FileManager.saveAsFile(), master.destroy()), master.destroy)

#Init buttons
def createButton(text, command):
	if "iter" not in createButton.__dict__:
		createButton.iter = 0

	Button(master, text = text, width = 8, command = command).grid(row = 1, column = createButton.iter, pady = 5)
	createButton.iter += 1

createButton("Open", FileManager.openFile)
createButton("Close", lambda: 
(
	Close() if FileManager.wasEdited else master.destroy()
))
createButton("Save", lambda: 
(
	None if not FileManager.wasEdited else FileManager.saveFile() if FileManager.fileName else FileManager.saveAsFile()
))
createButton("Insert", lambda: Insert())
createButton("Delete", lambda: 
(
	dataList.delete(dataList.selection()),
	[None for FileManager.wasEdited in [True]]
))
createButton("Search", lambda: Search())

master.mainloop()