async function loadStudents() {
    try {
        const response = await fetch('/api/student/all');
        const data = await response.json();
        const tbody = document.querySelector('#studentTable tbody');
        tbody.innerHTML = '';
        data.forEach(stu => {
            tbody.innerHTML += `
                <tr>
                    <td>${stu.id}</td>
                    <td>${stu.name}</td>
                    <td>${stu.age}</td>
                    <td>${stu.course}</td>
                    <td>${stu.marks}</td>
                    <td><button onclick="deleteStudent(${stu.id})">Delete</button></td>
                </tr>
            `;
        });
    } catch (e) {
        console.error('Failed to load students:', e);
        throw e;
    }
}

document.getElementById('addStudentForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const student = {
        id: Number.parseInt(document.getElementById('stuId').value),
        name: document.getElementById('stuName').value,
        age: Number.parseInt(document.getElementById('stuAge').value),
        course: document.getElementById('stuCourse').value,
        marks: Number.parseFloat(document.getElementById('stuMarks').value)
    };

    await fetch('/api/student/add', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(student)
    });
    document.getElementById('addStudentForm').reset();
    loadStudents();
});

async function deleteStudent(id) {
    await fetch(`/api/student/delete?id=${id}`, { method: 'DELETE' });
    loadStudents();
}

await loadStudents();
