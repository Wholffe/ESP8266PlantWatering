// dummy data
const data = [40, 38, 36, 30, 32, 34, 40, 50, 65, 100];

function createGraph(data) {
    const ctx = document.getElementById('dataChart').getContext('2d');
    const container = document.getElementById('graphContainer');
    const containerWidth = container.offsetWidth;
    const containerHeight = container.offsetHeight;

    const labelMargin = 20;

    const dataChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: data.map((_, i) => ` `), // x,y - label
            datasets: [{
                label: 'Data',
                data: data,
                borderColor: '#3498db', // light blue
                backgroundColor: 'rgba(52, 152, 219, 0.2)',
                borderWidth: 2,
                pointBackgroundColor: '#fff', // white
                pointBorderColor: '#3498db', // light blue
                pointRadius: 4,
                pointHoverRadius: 6,
                tension: 0.4
            }]
        },
        options: {
            maintainAspectRatio: false,
            layout: {
                padding: {
                    left: labelMargin,
                    right: labelMargin,
                    top: labelMargin,
                    bottom: labelMargin
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    grid: {
                        color: 'rgba(0, 0, 0, 0.15)', // small grid
                        borderDash: [5, 5]
                    }
                },
                x: {
                    grid: {
                        display: false // no grid x axes
                    }
                }
            },
            plugins: {
                legend: {
                    display: false
                }
            }
        }
    });

    dataChart.canvas.width = containerWidth;
    dataChart.canvas.height = containerHeight;
}

window.onload = function() {
    createGraph(data);
};