import random
import sqlite3
import requests
import json

response = requests.get("https://opentdb.com/api.php?amount=50&type=multiple")
text = response.text
data = json.loads(text)

con = sqlite3.connect("triviaDB.sqlite")

for ques in data["results"]:

    query = "insert into QUESTIONS (QUESTION, CORRECT, WRONG1, WRONG2, WRONG3) VALUES('{}', '{}', '{}', '{}', '{}')".format(ques["question"], ques["correct_answer"], ques["incorrect_answers"][0], ques["incorrect_answers"][1], ques["incorrect_answers"][2]).replace("&quot;", "\"").replace("&#039;", "''")
    print(query)
    res = con.execute(query)
    con.commit()
    res.fetchone()
con.close()