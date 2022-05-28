<template>
    <div class="sensor-graph">
        
        <h2>Evolution of values for {{ name }}</h2>
        <p>Here you can access the graphs highlighting the evolution of the sensoor measurements over time</p>

        <p>{{ info }}</p>

        <Bar
            :chart-options="chartOptions"
            :chart-data="chartData"
            :chart-id="chartId"
            :dataset-id-key="datasetIdKey"
            :plugins="plugins"
            :css-classes="cssClasses"
            :styles="styles"
            :width="100"
            :height="50"
        />
    </div>
</template>


<script>
import axios from 'axios';
import { Bar } from 'vue-chartjs';
import { Chart as ChartJS, Title, Tooltip, Legend, BarElement, CategoryScale, LinearScale } from 'chart.js';

ChartJS.register(Title, Tooltip, Legend, BarElement, CategoryScale, LinearScale);

export default {
    name: 'senor-graph',
    components: { Bar },
    props: {
        topic : {
            type: String,
            default: '/iot/water'
        },
        name : {
            type: String,
            default: 'data'
        },

        chartId: {
            type: String,
            default: 'bar-chart'
        },
        datasetIdKey: {
            type: String,
            default: 'label'
        },
        width: {
            type: Number,
            default: 400
        },
        height: {
            type: Number,
            default: 400
        },
        cssClasses: {
            default: '',
            type: String
        },
        styles: {
            type: Object,
            default: () => {}
        },
        plugins: {
            type: Object,
            default: () => {}
        }
    },

    data() {
        return {
            chartData: {
                labels: ['January', 'February', 'March'],
                datasets: [{data: [40, 20, 12]}]
            },
            chartOptions: {
                responsive: true
            },
            response: null
        }
    },
    
    methods: {
        fetchData(topic) {
            this.fetchInterval = setInterval(() => {
                axios.get('http://localhost:3000/api', {
                    params: { topic: topic }
                }).then(response => {
                    this.info = response;
                    console.log(response);
                }).catch(err => {
                    console.log(err);
                });
            }, 3000);            
        }
    },

    created() {
        this.fetchData();
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
</style>