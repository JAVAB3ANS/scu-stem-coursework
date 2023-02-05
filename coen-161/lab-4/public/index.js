//Question 1: the defer attribute is most preferred in ensuring e this script does not block the
//browser while parsing the JavaScript
//Question 2: their isn't a response yet by the time we need to log out Chart
//Question 3: Set interval is asynchronous and lets us create short time bursts after which each time something gets done
//Question 4: without clearInterval , one would not be able to cancel out the interval at specific moments when we need to
//in this case when we confirm that the Chart object exists
//Question 5: 100 stands for 100 milliseconds time intervak
let interval = setInterval(function main() {
  createGradeByWeekChart();
  createPointsInClassChart();
  createPointsByCategoryChart();
  if (!Chart) {
    console.log(Chart);
  } else {
    clearInterval(interval);
  }
}, 100);

function createGradeByWeekChart() {
  const canvas = document.querySelector("#WorkOrder");
  options = {
    // the type of chart we want to use
    type: "line",
    data: {
      // which axis shows these labels?
      labels: ["Quizzes", "Labs", "Theory", "Practice"],
      datasets: [
        // is this the top or bottom data set?
        // what happens if there's only 1 dataset?
        {
          label: "Earned",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "rgba(155, 89, 182, 1.0)",
        },
        {
          label: "Missed",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "rgba(155, 89, 182, 0.5)",
        },
        {
          label: "Ungraded",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "#eee",
        },
      ],
    },
    options: {
      scales: {
        y: {
          stacked: true,
          grid: {
            display: false,
          },
        },
      },
      x: {
        stacked: true,
        grid: {
          display: false,
        },
      },
      aspectRatio: 1,
      plugins: {
        title: {
          display: true,
          text: "Points by Category",
        },
        legend: {
          display: false,
        },
      },
    },
  };
  const chart = new Chart(canvas.getContext("2d"), options);
}
function createPointsInClassChart() {
  const canvas = document.querySelector("#Class");
  options = {
    // the type of chart we want to use
    type: "pie",
    data: {
      // which axis shows these labels?
      labels: ["Quizzes", "Labs", "Theory", "Practice"],
      datasets: [
        // is this the top or bottom data set?
        // what happens if there's only 1 dataset?
        {
          label: "Earned",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "rgba(155, 89, 182, 1.0)",
        },
        {
          label: "Missed",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "rgba(155, 89, 182, 0.5)",
        },
        {
          label: "Ungraded",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "#eee",
        },
      ],
    },
    options: {
      scales: {
        y: {
          stacked: true,
          grid: {
            display: false,
          },
        },
      },
      x: {
        stacked: true,
        grid: {
          display: false,
        },
      },
      aspectRatio: 1,
      plugins: {
        title: {
          display: true,
          text: "Points by Category",
        },
        legend: {
          display: false,
        },
      },
    },
  };
  const chart = new Chart(canvas.getContext("2d"), options);
}
function createPointsByCategoryChart() {
  const canvas = document.querySelector("#Category");
  options = {
    // the type of chart we want to use
    type: "bar",
    data: {
      // which axis shows these labels?
      labels: ["Quizzes", "Labs", "Theory", "Practice"],
      datasets: [
        // is this the top or bottom data set?
        // what happens if there's only 1 dataset?
        {
          label: "Earned",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "rgba(155, 89, 182, 1.0)",
        },
        {
          label: "Missed",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "rgba(155, 89, 182, 0.5)",
        },
        {
          label: "Ungraded",
          data: [65, 59, 80, 81, 56, 55, 40],
          backgroundColor: "#eee",
        },
      ],
    },
    options: {
      scales: {
        y: {
          stacked: true,
          grid: {
            display: false,
          },
        },
      },
      x: {
        stacked: true,
        grid: {
          display: false,
        },
      },
      aspectRatio: 1,
      plugins: {
        title: {
          display: true,
          text: "Points by Category",
        },
        legend: {
          display: false,
        },
      },
    },
  };
  const chart = new Chart(canvas.getContext("2d"), options);
}
