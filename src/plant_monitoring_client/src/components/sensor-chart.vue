<template>
    <div class="sensor-graph">      
        <h2>Real-time chart for the measurements of the {{ name }} sensor</h2>

        <apexchart
            :type="line"
            :options="chartOptions"
            :series="series">
        </apexchart>
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
                    id: 'vuechart'
                },
                color: '#FEB019'
            },
            series: [
                {
                    name: this.name,
                    data: [],
                },
            ],
            colors: [
                '#008FFB',
                '#00E396',
                '#FEB019',
                '#FF4560',
                '#775DD0'
            ]
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
.sensor-graph {
    margin-bottom: 300px;
}

button {
    flex-direction: column;
    align-items: center;
    padding: 6px 14px;
    font-family: -apple-system, BlinkMacSystemFont, 'Roboto', sans-serif;
    border-radius: 6px;
    border: none;

    color: #fff;
    background: linear-gradient(180deg, #4B91F7 0%, #367AF6 100%);
    background-origin: border-box;
    box-shadow: 0px 0.5px 1.5px rgba(54, 122, 246, 0.25), inset 0px 0.8px 0px -0.25px rgba(255, 255, 255, 0.2);
    user-select: none;
    -webkit-user-select: none;
    touch-action: manipulation;
}

button:focus {
  box-shadow: inset 0px 0.8px 0px -0.25px rgba(255, 255, 255, 0.2), 0px 0.5px 1.5px rgba(54, 122, 246, 0.25), 0px 0px 0px 3.5px rgba(58, 108, 217, 0.5);
  outline: 0;
}
</style>