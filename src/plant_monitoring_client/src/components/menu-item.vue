<template>
    <div class = "data-button">
        <button>{{ buttonLabel }}</button>
    </div>
    
</template>

<script>
export default {
    props: {
        buttonLabel: {
            type: String,
            default: 'Button'
        },
        topic: {
            type: String,
            default: ''
        }
    }
}
</script>