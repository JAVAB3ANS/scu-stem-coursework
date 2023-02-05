const EVENTS = {
  // september
  8: {
    21: {
      type: "office-hours",
    },
    22: {
      type: "homework",
      description: "Homework 1 Due",
    },
    24: {
      type: "office-hours",
    },
    28: {
      type: "homework",
      description: "Homework 2 Due",
    },
    30: {
      type: "lab",
      description: "Lab 1 Due",
    },
  },
  // october
  9: {
    3: {
      type: "office-hours",
    },
    4: {
      type: "homework",
      description: "Homework 3 Due",
    },
    5: {
      type: "lab",
      description: "Lab 2 Due",
    },
    10: {
      type: "office-hours",
    },
    13: {
      type: "quiz",
      description: "Quiz 1",
    },
    16: {
      type: "homework",
      description: "Homework 4 due",
    },
    17: {
      type: "office-hours",
    },
    19: {
      type: "lab",
      description: "Lab 3 Due",
    },
    24: {
      type: "office-hours",
    },
    28: {
      type: "homework",
      description: "Homework 5 due",
    },
  },
  // november
  10: {
    6: {
      type: "office-hours",
    },
    7: {
      type: "homework",
      description: "Homework 6 due",
    },
    9: {
      type: "lab",
      description: "Lab 4 Due",
    },
    13: {
      type: "office-hours",
    },
    16: {
      type: "lab",
      description: "Lab 5 Due",
    },
    21: {
      type: "homework",
      description: "Homework 7 due",
    },
    22: {
      type: "quiz",
      description: "Quiz 2",
    },
    30: {
      type: "office-hours",
    },
  },
  // december
  11: {
    1: {
      type: "lab",
      description: "Lab6Due",
    },
    2: {
      type: "quiz",
      description: "Quiz3",
    },
    4: {
      type: "homework",
      description: "Homework8due",
    },
  },
};

const today = new Date();
const target = document.getElementById("calendar-days");

document.getElementById("sept").addEventListener("click", rerender8);
document.getElementById("oct").addEventListener("click", rerender9);
document.getElementById("nov").addEventListener("click", rerender10);
document.getElementById("dec").addEventListener("click", rerender11);
function rerender8() {
  target.innerHTML = "";
  today.setMonth(document.getElementById("sept").name);
  displayMonth();
}
function rerender9() {
  target.innerHTML = "";
  today.setMonth(document.getElementById("oct").name);
  displayMonth();
}
function rerender10() {
  target.innerHTML = "";
  today.setMonth(document.getElementById("nov").name);
  displayMonth();
}
function rerender11() {
  target.innerHTML = "";
  today.setMonth(document.getElementById("dec").name);
  displayMonth();
}

function displayMonth() {
  const firstDay = DateFns.startOfMonth(today).getDay();
  const currentMonth = DateFns.startOfMonth(today).getMonth();
  const lastDayIndex = DateFns.endOfMonth(today).getDay();
  const lasttDay = DateFns.endOfMonth(today).getDate();

  const selectedMonth = EVENTS[currentMonth];

  const skip = firstDay;

  for (let index = 0; index < skip; index++) {
    target.innerHTML += "<div class='day'><p></p></div>";
  }
  for (let index = 1; index <= lasttDay; index++) {
    for (const key in selectedMonth) {
      if (key == index) {
        if (selectedMonth[key].type == "office-hours") {
          target.innerHTML += `<div class='day work-hours'><p>${index}</p></div>`;
        } else if ((selectedMonth[key] = "homework")) {
          target.innerHTML += `<div class='day homework'><p>${index}</p></div>`;
        } else if (selectedMonth[key] == "quiz") {
          target.innerHTML += `<div class='day quiz'><p>${index}</p></div>`;
        } else {
          target.innerHTML += `<div class='day lab'><p>${index}</p></div>`;
        }
      }
    }
    target.innerHTML += `<div class='day'><p>${index}</p></div>`;
  }
  for (let index = 0; index < lastDayIndex; index++) {
    target.innerHTML += "<div class='day'><p></p></div>";
  }
}
displayMonth();
