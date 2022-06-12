<template>
    <div class="sensor-chart-area">      
        <h2>Real-time line chart for the {{ name }} sensor</h2>

        <div class="chart">   
        <apexchart
            :type="line"
            :options="chartOptions"
            :series="series"
            :width="1000"
            :height="500">
        </apexchart>
        </div>
    <button ref="freezeButton" @click="freezeChart">Freeze chart</button>
    </div>
</template>


<script>
import axios from 'axios';
import VueApexCharts from "vue3-apexcharts";


export default {
    name: 'sensor-chart',
    components: { 
        apexchart: VueApexCharts
    },

    props: {
        topic : {
            type: String,
            default: '/iot/water'
        },
        name : {
            type: String,
            default: 'Sensor'
        },
    },

    data: function() {
        return {
            type: 'line',
            frozen: false,
            chartOptions: {
                chart: {
                    width: 1000,
                    height: 500
                },
                responsive: [
                    {
                        breakpoint: 480,
                        options: {
                        chart: {
                            width: 200,
                        },
                        legend: {
                            position: "bottom",
                        },
                        },
                    },
                ],
                colors: ['#990014'],
                fill: {
                    type: 'gradient'
                },
                stroke: {
                    width: 5
                }
            },
            series: [
                {
                    name: this.name,
                    data: [],
                },
            ],
            
        };
    },
    
    methods: {
        // Updates the MQTT data from the server every 3 seconds
        fetchData(topic) {
            this.fetchInterval = setInterval(() => {
                axios.get('http://localhost:3000/api?topic=' + topic).then(response => {
                    if(!this.frozen) {
                        this.series[0].data = response.data.messages;
                    }                 
                    console.log(response.data.messages);
                }).catch(err => {
                    console.log(err);
                });
            }, 10000);            
        },

        freezeChart() {
            this.$refs.freezeButton.innerText = this.frozen ? 'Freeze chart' : 'Unfreeze chart';
            this.frozen = !this.frozen;
        }
    },

    created() {
        this.fetchData(this.topic);
    },

    beforeUnmount() {
        clearInterval(this.fetchInterval);
    }
}
</script>


<style scoped>
.sensor-chart-area { 
    margin: auto;
    width:60%;
    margin-bottom: 150px;
}

.chart {
    margin: auto;
    width:60%;
}

button {
    width: 90%;
    flex-direction: column;
    align-items: center;
    padding: 6px 14px;
    font-family: -apple-system, BlinkMacSystemFont, 'Roboto', sans-serif;
    border-radius: 6px;
    border: none;

    color: #FDFFE7;
    background: #393A3C;
    background-origin: border-box;
    box-shadow: 0px 0.5px 1.5px rgba(54, 122, 246, 0.25), inset 0px 0.8px 0px -0.25px rgba(255, 255, 255, 0.2);
    user-select: none;
    -webkit-user-select: none;
    touch-action: manipulation;
}

button:focus {
  box-shadow: inset 0px 0.8px 0px -0.25px rgba(255, 255, 255, 0.2), 0px 0.5px 1.5px rgba(54, 122, 246, 0.25), 0px 0px 0px 3.5px #393A3C;
  outline: 0;
}
</style>